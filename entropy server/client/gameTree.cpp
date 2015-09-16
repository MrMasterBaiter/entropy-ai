#include<iostream>
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
	vector<vector<int> > colorDistance;
	state(){
		depth=0;
		probability=1;
		board=new int*[board_Size];
		for(int i=0;i<board_Size;i++){
			board[i]=new int[board_Size];
		}
		for(int i=0;i<board_Size;i++){
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
	

	double utility(int count){
		//numPalindrome();
		double d1=heuristic(count);
		// //numPalindrome();
		double d2=palindromicScore();
		// numPalindrome();

		return (d1+d2);
		// return palindromicScore();
	}
	double heuristic(int count){
		double score=0;
		double expectedScore[board_Size-1];
		// double frequencyRow[board_Size-1];
		// double frequencyCol[board_Size-1];
		if(board_Size==5){
			expectedScore[0]=5.5;
			expectedScore[1]=10;
			expectedScore[2]=2;
			expectedScore[3]=2;
			
		}
		else if(board_Size==7){
			expectedScore[0]=5;
			expectedScore[1]=6;
			expectedScore[2]=1;
			expectedScore[3]=0;
			expectedScore[4]=2.5;
			expectedScore[5]=1.7;
		}
		for(int i=0;i<board_Size;i++){
			int gap=board_Size-2;
			int mid=board_Size/2;
			while(gap>=0){
				for(int j=0;((j+gap+1)<board_Size);j++){
					if(board[i][j]!=-1){
						if(board[i][j]==board[i][j+gap+1]){
							score+=expectedScore[gap];
							
						}
					}
					if(board[j][i]!=-1){
						if(board[j][i]==board[j+gap+1][i]){
							score+=expectedScore[gap];
						}
					}
				}
				gap--;
			}
		}
		return score;
	}
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
							if(palindromeLength == 2){
								score += palindromeLength;
							}
							else if(palindromeLength == 4){
								if(board[row][indexLeft] == board[row][indexLeft+1]){
									score += 10;  
								}
								else{
									score += 4;
								}
							}
							myBool = 1;
							// cout<<"row "<<row<<" indexLeft "<<indexLeft<<" indexRight "<<indexRight<<" length "<<palindromeLength<<endl;
						}
						palindromeLength++;
						indexLeft--;
						if (indexLeft >= 0){
							if (board[row][indexLeft] == board[row][indexRight]){
								if(palindromeLength == 3){
									if(board[row][indexLeft] == board[row][indexLeft+1]){
										score += 7;
									}
									else{
										score += 3;
									}
								}
								else if(palindromeLength == 5){
									if(board[row][indexLeft] == board[row][indexLeft+1]){
										if(board[row][indexLeft] == board[row][indexLeft+2]){
											score += 30;
										}
										else{
											score += 12;
										}
									}
									else if(board[row][indexLeft] == board[row][indexLeft+2]){
										score += 14;
									}
									else{
										score += 8;
									}
								}
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
							if(palindromeLength == 2){
								score += palindromeLength;
							}
							else if(palindromeLength == 4){
								if(board[indexTop][col] == board[indexTop+1][col]){
									score += 16;
								}
								else{
									score += 4;
								}
							}
							myBool = 1;
							// cout<<"indexTop "<<indexTop<<" indexBot "<<indexBot<<" col "<<col<<" length "<<palindromeLength<<endl;
						}
						palindromeLength++;
						indexTop--;
						if (indexTop >= 0){
							if (board[indexTop][col] == board[indexBot][col]){
								if(palindromeLength == 3){
									if(board[row][indexLeft] == board[row][indexLeft+1]){
										score += 7;
									}
									else{
										score += 3;
									}
								}
								else if(palindromeLength == 5){
									if(board[indexTop][col] == board[indexTop+1][col]){
										if(board[indexTop][col] == board[indexTop+2][col]){
											score += 30;
										}
										else{
											score += 12;
										}
									}
									else if(board[indexTop][col] == board[indexTop+2][col]){
										score += 14;
									}
									else{
										score += 5;
									}
								}
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
};

