#include <iostream>
#include <vector>
using namespace std;

enum player{Min,Max};
static int board_Size;

class action{
public:
	 int x,y,x1,y1;
	 action(){

	 }
	 action(int i,int j,int k,int l){
		x=i;y=j;x1=k;y1=l;
	}
};
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
	vector <vector <int> > colorDistance;

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
		return stateScore;
		// return palindromicScore();
	}
	/*
	int palindromicScore(){
		int score=0;
		for(int row=0;row<board_Size;row++){
			bool b=false;
			int first,second;
			for(int i=0;i<board_Size;i++){
				if(board[row][i]!=-1){
					if(!b){
						first=i;
						second=i;
						b=true;
					}
					else{
						second=i;
					}
				}
				else{
					if(b){
						score+=sumPalindrome(row,first,row,second);
						b=false;
					}
				}
			}
			if(b){
				score+=sumPalindrome(row,first,row,second);
			}
		}
		for(int column=0;column<board_Size;column++){
			bool b=false;
			int first,second;
			for(int i=0;i<board_Size;i++){
				if(board[i][column]!=-1){
					if(!b){
						first=i;
						second=i;
						b=true;
					}
					else{
						second=i;
					}
				}
				else{
					if(b){
						score+=sumPalindrome(first,column,second,column);
						b=false;
					}
				}
			}
			if(b){
				score+=sumPalindrome(first,column,second,column);
			}
		}
		return score;
	}

	int sumPalindrome(int i,int j, int k, int l){
		int score=0;
		if(i==k){
			int count =2;
			int size=l-j+1;
			while(count<=size){
				int j1=j;
				while(j1+count-1<=l){
					if(isPalindrome(i,j1,k,(j1+count-1))){
						score+=count;
					}
					j1++;
				}
				count++;
			}
		}
		else if(j==l){
			int count =2;
			int size=k-i+1;
			while(count<=size){
				int i1=i;
				while(i1+count-1<=k){
					if(isPalindrome(i1,j,(i1+count-1),l)){
						score+=count;
					}
					i1++;
				}
				count++;
			}
		}
		return score;
	}

	bool isPalindrome(int i,int j, int k, int l){
		if(i>=k && j>=l){
			return true;
		}
		else if(i==k){
			// assume j<=l
			if(board[i][j]==board[k][l]){
				return isPalindrome(i,j+1,k,l-1);
			}
			else{
				return false;
			}
		}
		else if(j==l){
			//assume i<=k
			if(board[i][j]==board[k][l]){
				return isPalindrome(i+1,j,k-1,l);
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
	*/

	void printvector(vector<action> v){
		int l=v.size();
		for(int i=0;i<l;i++){
			action a=v.at(i);
			cout<<"("<<a.x<<","<<a.y<<"),("<<a.x1<<","<<a.y1<<")"<<endl;
		}
		cout<<"new vector"<<endl;
	}

	void printboard()
	{
		for(int i=0;i<board_Size;i++){
			for(int j=0;j<board_Size;j++){
				if(board[i][j]==-1){
					cout<<" "<<" | ";
				}
				else{
					cout<<board[i][j]<<" | ";
				}

			}
			cout<<endl;
		}
		cout<<endl<<endl;
	}

	void numPalindrome(){
		vector <vector <int> > tempVector(board_Size, std::vector<int>(board_Size));
		for(int ii = 0; ii < board_Size; ii++){
			for(int jj = 0; jj < board_Size; jj++){
				tempVector[ii][jj] = -1;
			}
		}
		int score = 0;
		int count = 0;
		int palindromeLength = 0;
		for (int row = 0; row < board_Size; row++){
			for (int col = 0; col < board_Size; col++){
				// color Distance Vector
				count = 0;
				if(board[row][col] != -1){
					while(1){
						if(tempVector[board[row][col]][count] == -1){
							tempVector[board[row][col]][count] = 10*row + col;
							break;
						}
						count++;
						if(count == 5){
							break;
						}
					}
				}
				bool myBool = 0;
				palindromeLength = 2;
				int indexLeft = col;
				int indexRight = col+1;
				while ((indexLeft >= 0) && (indexRight <= board_Size-1)){
					myBool = 0;
					if (board[row][indexRight] != -1){
						if (board[row][indexLeft] == board[row][indexRight]){
							score += palindromeLength;
							myBool = 1;
							// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
						}
						palindromeLength++;
						indexLeft--;
						if (indexLeft >= 0){
							if (board[row][indexLeft] == board[row][indexRight]){
								score += palindromeLength;
								myBool = 1;
								// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
							}
						}
						if (myBool == 0){
							break;
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
					myBool = 0;
					if (board[indexBot][col] != -1){
						if (board[indexTop][col] == board[indexBot][col]){
							score += palindromeLength;
							myBool = 1;
							// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
						}
						palindromeLength++;
						indexTop--;
						if (indexTop >= 0){
							if (board[indexTop][col] == board[indexBot][col]){
								score += palindromeLength;
								myBool = 1;
								// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
							}
						}
						if (myBool == 0){
							break;
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
		this->colorDistance = tempVector;
		//return this->stateScore;
	}

	/*
	void updateScore(action a){
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
		row = a.x1;
		col = a.y1;
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
	*/
	
	vector <action> threesActions(){
		vector <action> retVector;
		int row1, row2, col1, col2;
		int val1, val2;
		for (int i = 0; i < board_Size-1; i++){
			for (int j = 0; j < board_Size-1; j++){
				if(this->colorDistance[i][j] != -1){
					val1 = this->colorDistance[i][j];
					row1 = val1/10;
					col1 = val1 % 10;
					for (int k = j+1; k < board_Size-1; k++){
						if (this->colorDistance[i][k] != -1){
							val2 = this->colorDistance[i][k];
							row2 = val2/10;
							col2 = val2 % 10;
							if (row1 == row2){
								if(col1 > col2){
									if((col1 - col2) == 1){
										if(col1 < board_Size-1){
											if(this->board[row1][col1+1] == -1){
												action a(row1, col1, row1, col1+1);
												retVector.push_back(a);
											}
										}
										if(col2 > 0){
											if(this->board[row1][col2-1] == -1){
												action a(row1, col2, row1, col2-1);
												retVector.push_back(a);
											}
										}
									}
									else if((col1 - col2) == 3){
										if(this->board[row1][col1-1] == -1){
											action a(row1, col1, row1, col1-1);
											retVector.push_back(a);
										}
										if(this->board[row1][col2+1] == -1){
											action a(row1, col2, row1, col2+1);
											retVector.push_back(a);
										}
									}
									else if((col1 - col2) == 4){
										if((this->board[row1][col1-1] == -1) && (this->board[row1][col1-2] == -1)){
											action a(row1, col1, row1, col1-2);;
											retVector.push_back(a);
										}
										if((this->board[row1][col2+1] == -1) && (this->board[row1][col2+2] == -1)){
											action a(row1, col2, row1, col2+2);
											retVector.push_back(a);
										}
									}
								}
								else if(col2 > col1){
									if((col2 - col1) == 1){
										if(col2 < board_Size-1){
											if(this->board[row1][col2+1] == -1){
												action a(row1, col2, row1, col2+1);
												retVector.push_back(a);
											}
										}
										if(col1 > 0){
											if(this->board[row1][col1-1] == -1){
												action a(row1, col1, row1, col1-1);
												retVector.push_back(a);
											}
										}
									}
									else if((col2 - col1) == 3){
										if(this->board[row1][col2-1] == -1){
											action a(row1, col2, row1, col2-1);
											retVector.push_back(a);
										}
										if(this->board[row1][col1+1] == -1){
											action a(row1, col1, row1, col1+1);
											retVector.push_back(a);
										}
									}
									else if((col2 - col1) == 4){
										if((this->board[row1][col2-1] == -1) && (this->board[row1][col2-2] == -1)){
											action a(row1, col2, row1, col2-2);;
											retVector.push_back(a);
										}
										if((this->board[row1][col1+1] == -1) && (this->board[row1][col1+2] == -1)){
											action a(row1, col1, row1, col1+2);
											retVector.push_back(a);
										}
									}
								}
							}
							// TO DO (col1 == col2)
							else if (col1 == col2){
								if(row1 > row2){
									if((row1 - row2) == 1){
										if(row1 < board_Size-1){
											if(this->board[row1+1][col1] == -1){
												action a(row1, col1, row1+1, col1);
												retVector.push_back(a);
											}
										}
										if(row2 > 0){
											if(this->board[row2-1][col1] == -1){
												action a(row2, col1, row2-1, col1);
												retVector.push_back(a);
											}
										}
									}
									else if((row1 - row2) == 3){
										if(this->board[row1-1][col1] == -1){
											action a(row1, col1, row1-1, col1);
											retVector.push_back(a);
										}
										if(this->board[row2+1][col1] == -1){
											action a(row2, col1, row2+1, col1);
											retVector.push_back(a);
										}
									}
									else if((row1 - row2) == 4){
										if((this->board[row1-1][col1] == -1) && (this->board[row1-2][col1] == -1)){
											action a(row1, col1, row1-2, col1);
											retVector.push_back(a);
										}
										if((this->board[row2+1][col1] == -1) && (this->board[row2+2][col1] == -1)){
											action a(row2, col1, row2+2, col1);
											retVector.push_back(a);
										}
									}
								}
								else if(row2 > row1){
									if((row2 - row1) == 1){
										if(row2 < board_Size-1){
											if(this->board[row2+1][col1] == -1){
												action a(row2, col1, row2+1, col1);
												retVector.push_back(a);
											}
										}
										if(row1 > 0){
											if(this->board[row1-1][col1] == -1){
												action a(row1, col1, row1-1, col1);
												retVector.push_back(a);
											}
										}
									}
									else if((row2 - row1) == 3){
										if(this->board[row2-1][col1] == -1){
											action a(row2, col1, row2-1, col1);
											retVector.push_back(a);
										}
										if(this->board[row1+1][col1] == -1){
											action a(row1, col1, row1+1, col1);
											retVector.push_back(a);
										}
									}
									else if((row2 - row1) == 4){
										if((this->board[row2-1][col1] == -1) && (this->board[row2-2][col1] == -1)){
											action a(row2, col1, row2-2, col1);
											retVector.push_back(a);
										}
										if((this->board[row1+1][col1] == -1) && (this->board[row1+2][col1] == -1)){
											action a(row1, col1, row1+2, col1);
											retVector.push_back(a);
										}
									}
								}
							}
							// TO DO (row1 != row2) && (col1 != col2)
							else{
								if(row1 > row2){
									if((row1 - row2) == 1){
										if(row1 < board_Size-1){
											if(this->board[row1+1][col1] == -1){
												action a(row1, col1, row1+1, col1);
												retVector.push_back(a);
											}
										}
										if(row2 > 0){
											if(this->board[row2-1][col1] == -1){
												action a(row2, col1, row2-1, col1);
												retVector.push_back(a);
											}
										}
									}
									else if((row1 - row2) == 3){
										if(this->board[row1-1][col1] == -1){
											action a(row1, col1, row1-1, col1);
											retVector.push_back(a);
										}
										if(this->board[row2+1][col1] == -1){
											action a(row2, col1, row2+1, col1);
											retVector.push_back(a);
										}
									}
									else if((row1 - row2) == 4){
										if((this->board[row1-1][col1] == -1) && (this->board[row1-2][col1] == -1)){
											action a(row1, col1, row1-2, col1);
											retVector.push_back(a);
										}
										if((this->board[row2+1][col1] == -1) && (this->board[row2+2][col1] == -1)){
											action a(row2, col1, row2+2, col1);
											retVector.push_back(a);
										}
									}
									if(col1 > col2){
										if((col1 - col2) == 1){
											if(col1 < board_Size-1){
												if(this->board[row1][col1+1] == -1){
													action a(row1, col1, row1, col1+1);
													retVector.push_back(a);
												}
											}
											if(col2 > 0){
												if(this->board[row1][col2-1] == -1){
													action a(row1, col2, row1, col2-1);
													retVector.push_back(a);
												}
											}
										}
										else if((col1 - col2) == 3){
											if(this->board[row1][col1-1] == -1){
												action a(row1, col1, row1, col1-1);
												retVector.push_back(a);
											}
											if(this->board[row1][col2+1] == -1){
												action a(row1, col2, row1, col2+1);
												retVector.push_back(a);
											}
										}
										else if((col1 - col2) == 4){
											if((this->board[row1][col1-1] == -1) && (this->board[row1][col1-2] == -1)){
												action a(row1, col1, row1, col1-2);;
												retVector.push_back(a);
											}
											if((this->board[row1][col2+1] == -1) && (this->board[row1][col2+2] == -1)){
												action a(row1, col2, row1, col2+2);
												retVector.push_back(a);
											}
										}
									}
									else{
										if((col2 - col1) == 1){
											if(col2 < board_Size-1){
												if(this->board[row1][col2+1] == -1){
													action a(row1, col2, row1, col2+1);
													retVector.push_back(a);
												}
											}
											if(col1 > 0){
												if(this->board[row1][col1-1] == -1){
													action a(row1, col1, row1, col1-1);
													retVector.push_back(a);
												}
											}
										}
										else if((col2 - col1) == 3){
											if(this->board[row1][col2-1] == -1){
												action a(row1, col2, row1, col2-1);
												retVector.push_back(a);
											}
											if(this->board[row1][col1+1] == -1){
												action a(row1, col1, row1, col1+1);
												retVector.push_back(a);
											}
										}
										else if((col2 - col1) == 4){
											if((this->board[row1][col2-1] == -1) && (this->board[row1][col2-2] == -1)){
												action a(row1, col2, row1, col2-2);;
												retVector.push_back(a);
											}
											if((this->board[row1][col1+1] == -1) && (this->board[row1][col1+2] == -1)){
												action a(row1, col1, row1, col1+2);
												retVector.push_back(a);
											}
										}
									}
								}
								else{
									if((row2 - row1) == 1){
										if(row2 < board_Size-1){
											if(this->board[row2+1][col1] == -1){
												action a(row2, col1, row2+1, col1);
												retVector.push_back(a);
											}
										}
										if(row1 > 0){
											if(this->board[row1-1][col1] == -1){
												action a(row1, col1, row1-1, col1);
												retVector.push_back(a);
											}
										}
									}
									else if((row2 - row1) == 3){
										if(this->board[row2-1][col1] == -1){
											action a(row2, col1, row2-1, col1);
											retVector.push_back(a);
										}
										if(this->board[row1+1][col1] == -1){
											action a(row1, col1, row1+1, col1);
											retVector.push_back(a);
										}
									}
									else if((row2 - row1) == 4){
										if((this->board[row2-1][col1] == -1) && (this->board[row2-2][col1] == -1)){
											action a(row2, col1, row2-2, col1);
											retVector.push_back(a);
										}
										if((this->board[row1+1][col1] == -1) && (this->board[row1+2][col1] == -1)){
											action a(row1, col1, row1+2, col1);
											retVector.push_back(a);
										}
									}
									if(col1 > col2){
										if((col1 - col2) == 1){
											if(col1 < board_Size-1){
												if(this->board[row1][col1+1] == -1){
													action a(row1, col1, row1, col1+1);
													retVector.push_back(a);
												}
											}
											if(col2 > 0){
												if(this->board[row1][col2-1] == -1){
													action a(row1, col2, row1, col2-1);
													retVector.push_back(a);
												}
											}
										}
										else if((col1 - col2) == 3){
											if(this->board[row1][col1-1] == -1){
												action a(row1, col1, row1, col1-1);
												retVector.push_back(a);
											}
											if(this->board[row1][col2+1] == -1){
												action a(row1, col2, row1, col2+1);
												retVector.push_back(a);
											}
										}
										else if((col1 - col2) == 4){
											if((this->board[row1][col1-1] == -1) && (this->board[row1][col1-2] == -1)){
												action a(row1, col1, row1, col1-2);;
												retVector.push_back(a);
											}
											if((this->board[row1][col2+1] == -1) && (this->board[row1][col2+2] == -1)){
												action a(row1, col2, row1, col2+2);
												retVector.push_back(a);
											}
										}
									}
									else{
										if((col2 - col1) == 1){
											if(col2 < board_Size-1){
												if(this->board[row1][col2+1] == -1){
													action a(row1, col2, row1, col2+1);
													retVector.push_back(a);
												}
											}
											if(col1 > 0){
												if(this->board[row1][col1-1] == -1){
													action a(row1, col1, row1, col1-1);
													retVector.push_back(a);
												}
											}
										}
										else if((col2 - col1) == 3){
											if(this->board[row1][col2-1] == -1){
												action a(row1, col2, row1, col2-1);
												retVector.push_back(a);
											}
											if(this->board[row1][col1+1] == -1){
												action a(row1, col1, row1, col1+1);
												retVector.push_back(a);
											}
										}
										else if((col2 - col1) == 4){
											if((this->board[row1][col2-1] == -1) && (this->board[row1][col2-2] == -1)){
												action a(row1, col2, row1, col2-2);;
												retVector.push_back(a);
											}
											if((this->board[row1][col1+1] == -1) && (this->board[row1][col1+2] == -1)){
												action a(row1, col1, row1, col1+2);
												retVector.push_back(a);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		return retVector;
	}
};


// void addNeighbours(int i,int j){
// 	neighbourList[i][j].isGood=false;
// 	for(int k=i+1;k<board_Size;k++){
// 		if(neighbourList[k][j].isGood=true){
// 			neighbourList[k][j].left.x=i;
// 			neighbourList[k][j].left.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// 	for(int k=i-1;k>=0;k--){
// 		if(neighbourList[k][j].isGood=true){
// 			neighbourList[k][j].right.x=i;
// 			neighbourList[k][j].right.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// 	for(int k=j+1;k<board_Size;k++){
// 		if(neighbourList[i][k].isGood=true){
// 			neighbourList[i][k].bottom.x=i;
// 			neighbourList[i][k].bottom.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// 	for(int k=j-1;k>=0;k--){
// 		if(neighbourList[i][k].isGood=true){
// 			neighbourList[i][k].top.x=i;
// 			neighbourList[i][k].top.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// }
// void removeNeighbours(int i,int j){
// 	neighbourList[i][j].isGood=true;
// 	for(int k=i+1;k<board_Size;k++){
// 		if(neighbourList[k][j].isGood=true){
// 			neighbourList[k][j].left.x=i;
// 			neighbourList[k][j].left.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// 	for(int k=i-1;k>=0;k--){
// 		if(neighbourList[k][j].isGood=true){
// 			neighbourList[k][j].right.x=i;
// 			neighbourList[k][j].right.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// 	for(int k=j+1;k<board_Size;k++){
// 		if(neighbourList[i][k].isGood=true){
// 			neighbourList[i][k].bottom.x=i;
// 			neighbourList[i][k].bottom.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// 	for(int k=j-1;k>=0;k--){
// 		if(neighbourList[i][k].isGood=true){
// 			neighbourList[i][k].top.x=i;
// 			neighbourList[i][k].top.y=j;
// 		}
// 		else{
// 			break;
// 		}
// 	}
// }
// action inintialOrderMove(){
// 	if(count<=3){
// 		if(count==1){
// 			int c=color[board_Size-1];
// 			position p=colorPositions[c][0];
// 			if(p.x!=0){
// 				action a(p.x,p.y,0,p.y);
// 				p.x=0;
// 				colorPositions[c][0]=p;
// 				return a;
// 			}
// 		}
// 		else if(count==2){
// 			int c1,c2;
// 			c1=color[board_Size-1];
// 			if(colorCount[board_Size-1]==2){
// 				c2=c1;
// 			}
// 			else{
// 				c2=color[board_Size-2];
// 			}
// 			if(c1==c2){
// 				position p1,p2;
// 				p1=colorPositions[c1][0];
// 				p2=colorPositions[c1][1];
// 				int x0=p2.x-p1.x;
// 				int y0=p2.y-p1.y;
// 				if(p1.y==0){
// 					if(p2.y==0){
// 						if(x0<0){
// 							action a(p2.x,p2.y,p1.x-1,p2.y);
// 							p2.x=p1.x-1;
// 							colorPositions[c1][1]=p2;
// 							return a;
// 						}
// 						else{
// 							action a(p2.x,p2.y,p1.x+1,p2.y);
// 							p2.x=p1.x+1;
// 							colorPositions[c1][1]=p2;
// 							return a;
// 						}
// 					}
// 					else if(x0==1 || x0==-1 || x0==2|| x0==-2){
// 						action a(p2.x,p2.y,p2.x,0);
// 						p2.y=0;
// 						colorPositions[c1][1]=p2;
// 						return a;
// 					}
// 					else{
// 						action a(p2.x,p2.y,p2.x,p1.y+1);
// 						p2.y=p1.y+1;
// 						colorPositions[c1][1]=p2;
// 						return a;
// 					}

// 				}
// 				else{
// 					if(p2.x==0){
// 						if(y0<0){

// 						}
// 						else{

// 						}
// 					}
// 					else if(y0==1 || y0==-1 || y0==2|| y0==-2){
// 						action a(p2.x,p2.y,0,p2.y);
// 						p2.x=0;
// 						colorPositions[c1][1]=p2;
// 						return a;
// 					}
// 				}
// 			}

// 		}
// 		else if(count==3){

// 		}
// 	}
// 	else{

// 	}
// }
// void initNeighbours(){
// 	neighbourList=new neighbours*[board_Size];
// 	for(int i=0;i<board_Size;i++){
// 		neighbourList[i]=new neighbours[board_Size];
// 	}
// }

// state* result(state* s, action a){
	// 	state* s1=new state(s);
	// 	int temp=s1->board[a.x][a.y];
	// 	s1->board[a.x][a.y]=-1;
	// 	s1->board[a.x1][a.y1]=temp;
	// 	s1->depth=s->depth+1;
	// 	return s1;
	// }
	// state* result2(state*s, action2 a){
	// 	state* s1=new state(s);
	// 	s1->board[a.x][a.y]=a.color;
	// 	s1->depth=s->depth+1;
	// 	s1->probability*=a.probability;
	// 	return s1;
	// }
