#include<iostream>
#include <vector>
using namespace std;

enum player{Min,Max};
//enum color{red,blue,yellow,green,orange,white,pink};

static int board_Size;

// Order's action
class action{
public:
	int x,y,x1,y1;
	action(){
	}

	action(int i,int j,int k,int l){
		x=i;y=j;x1=k;y1=l;
	}
};

// Chaos's action
class action2{
public:
	int x,y;
	int color;
	double probability;
	action2(){
	}

	action2(int i,int j,int  col,double p){
		x=i;
		y=j;
		color=col;
		probability=p;
	}
};

class state{
public:
	int **board;
	int depth;
	double probability;
	int stateScore;
	state *parentState;
	
	state(){
		depth=0;
		probability=1;
		board=new int*[board_Size];
		for(int i=0;i<board_Size;i++){
			board[i]=new int[board_Size];
			for(int j=0;j<board_Size;j++){
				board[i][j]=-1;
			}
		}
	}

	state(state* s){
		board=new int*[board_Size];
		for(int i=0;i<board_Size;i++){
			board[i]=new int[board_Size];
			for(int j=0;j<board_Size;j++){
				board[i][j]=s->board[i][j];
			}
		}
		probability=s->probability;
		depth=s->depth;
	}

	~state(){
		for(int i=0;i<board_Size;i++){
			delete[] board[i];
		}
		delete[] board;
	}

	int utility(){
		numPalindrome();
		return this->stateScore;
	}

	vector<action> orderActions(){
		vector<action> actions;
		action a(0,0,0,0);
		actions.push_back(a);

		for(int i=0;i<board_Size;i++){
			for(int j=0;j<board_Size;j++){
				if(board[i][j]!=-1){
					vector<action> right,left,up,down;
					right=moveRight(i,j);
					left=moveLeft(i,j);
					up=moveUp(i,j);
					down=moveDown(i,j);

					actions.insert(actions.end(),right.begin(),right.end());
					actions.insert(actions.end(),left.begin(),left.end());
					actions.insert(actions.end(),up.begin(),up.end());
					actions.insert(actions.end(),down.begin(),down.end());
				}
			}
		}
		return actions;
	}

	vector<action> moveRight(int i,int j){
		int k=i+1;
		vector<action> actions;
		while(k<board_Size){
			if(board[k][j]==-1){
				action a(i,j,k,j);
				actions.push_back(a);
				k++;
			}
			else{
				break;
			}

		}
		return actions;
	}

	vector<action> moveLeft(int i,int j){
		int k=i-1;
		vector<action> actions;
		while(k>=0){
			if(board[k][j]==-1){
				action a(i,j,k,j);
				actions.push_back(a);
				k--;
			}
			else{
				break;
			}
		}
		return actions;

	}

	vector<action> moveUp(int i,int j){
		int k=j+1;
		vector<action> actions;
		while(k<board_Size){
			if(board[i][k]==-1){
				action a(i,j,i,k);
				actions.push_back(a);
				k++;
			}
			else{
				break;
			}
		}
		return actions;

	}

	vector<action> moveDown(int i,int j){
		int k=j-1;
		vector<action> actions;
		while(k>=0){
			if(board[i][k]==-1){
				action a(i,j,i,k);
				actions.push_back(a);
				k--;
			}
			else{
				break;
			}
		}
		return actions;
	}

	void numPalindrome(){
		int score = 0;
		int palindromeLength = 0;
		for (int row = 0; row < board_Size; row++){
			for (int col = 0; col < board_Size; col++){
				palindromeLength = 2;
				int indexLeft = col;
				int indexRight = col+1;
				while ((indexLeft >= 0) && (indexRight <= board_Size-1)){
					if (board[row][indexRight] != -1){
						if (board[row][indexLeft] == board[row][indexRight]){
							score += palindromeLength;
							// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
						}
						palindromeLength++;
						indexLeft--;
						if (indexLeft >= 0){
							if (board[row][indexLeft] == board[row][indexRight]){
								score += palindromeLength;
								// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
							}
						}
						palindromeLength++;
						indexRight++;
					}
					else{
						indexLeft--;
						indexRight++;
						palindromeLength += 2;
					}
				}

				palindromeLength = 2;
				int indexTop = row;
				int indexBot = row+1;
				while ((indexTop >= 0) && (indexBot <= board_Size-1)){
					if (board[indexBot][col] != -1){
						if (board[indexTop][col] == board[indexBot][col]){
							score += palindromeLength;
							// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
						}
						palindromeLength++;
						indexTop--;
						if (indexTop >= 0){
							if (board[indexTop][col] == board[indexBot][col]){
								score += palindromeLength;
								// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
							}
						}
						palindromeLength++;
						indexBot++;
					}
					else{
						palindromeLength += 2;
						indexTop--;
						indexBot++;
					}
				}
				// cout<<"score = "<<score<<endl;
			}
		}
		this->stateScore = score;
		//return this->stateScore;
	}

	void updateScore(action2 a){
		int tempScore = parentState->stateScore;
		int row = a.x;
		int col = a.y;
		for(int i = 0; i < board_Size; i++){	
			int palindromeLength = 2;
			int indexLeft = i;
			int indexRight = i+1;
			while ((indexLeft >= 0) && (indexRight <= board_Size-1)){
				if (parentState->board[row][indexRight] != -1){
					if (parentState->board[row][indexLeft] == parentState->board[row][indexRight]){
						tempScore -= palindromeLength;
						// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
					}
					palindromeLength++;
					indexLeft--;
					if (indexLeft >= 0){
						if (parentState->board[row][indexLeft] == parentState->board[row][indexRight]){
							tempScore -= palindromeLength;
							// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
						}
					}
					palindromeLength++;
					indexRight++;
				}
				else{
					indexLeft--;
					indexRight++;
					palindromeLength += 2;
				}
			}
		}
		for(int i = 0; i < board_Size; i++){
			int palindromeLength = 2;
			int indexTop = i;
			int indexBot = i+1;
			while ((indexTop >= 0) && (indexBot <= board_Size-1)){
				if (parentState->board[indexBot][col] != -1){
					if (parentState->board[indexTop][col] == parentState->board[indexBot][col]){
						tempScore -= palindromeLength;
						// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
					}
					palindromeLength++;
					indexTop--;
					if (indexTop >= 0){
						if (parentState->board[indexTop][col] == parentState->board[indexBot][col]){
							tempScore -= palindromeLength;
							// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
						}
					}
					palindromeLength++;
					indexBot++;
				}
				else{
					palindromeLength += 2;
					indexTop--;
					indexBot++;
				}
			}
		}

		for(int i = 0; i < board_Size; i++){	
			int palindromeLength = 2;
			int indexLeft = i;
			int indexRight = i+1;
			while ((indexLeft >= 0) && (indexRight <= board_Size-1)){
				if (this->board[row][indexRight] != -1){
					if (this->board[row][indexLeft] == this->board[row][indexRight]){
						tempScore += palindromeLength;
						// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
					}
					palindromeLength++;
					indexLeft--;
					if (indexLeft >= 0){
						if (this->board[row][indexLeft] == this->board[row][indexRight]){
							tempScore += palindromeLength;
							// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
						}
					}
					palindromeLength++;
					indexRight++;
				}
				else{
					indexLeft--;
					indexRight++;
					palindromeLength += 2;
				}
			}
		}
		for(int i = 0; i < board_Size; i++){
			int palindromeLength = 2;
			int indexTop = i;
			int indexBot = i+1;
			while ((indexTop >= 0) && (indexBot <= board_Size-1)){
				if (this->board[indexBot][col] != -1){
					if (this->board[indexTop][col] == this->board[indexBot][col]){
						tempScore += palindromeLength;
						// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
					}
					palindromeLength++;
					indexTop--;
					if (indexTop >= 0){
						if (this->board[indexTop][col] == this->board[indexBot][col]){
							tempScore += palindromeLength;
							// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
						}
					}
					palindromeLength++;
					indexBot++;
				}
				else{
					palindromeLength += 2;
					indexTop--;
					indexBot++;
				}
			}
		}
		this->stateScore = tempScore;
	}

	void printvector(vector<action> v){
		int l=v.size();
		for(int i=0;i<l;i++){
			action a=v.at(i);
			cout<<"("<<a.x<<","<<a.y<<"),("<<a.x1<<","<<a.y1<<")"<<endl;
		}
		cout<<"new vector"<<endl;
	}

	void printboard(){
		for(int i=0;i<board_Size;i++){
			for(int j=0;j<board_Size;j++){
				cout<<board[i][j]<<" | ";
			}
			cout<<endl;
		}
		cout<<endl<<endl;
	}
};
