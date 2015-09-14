#include <iostream>
#include <vector>
using namespace std;

enum player{Min,Max};
//enum color{red,blue,yellow,green,orange,white,pink};

static int board_Size;

int mod(int x){
	if(x >= 0){
		return x;
	}
	else{
		return -1*x;
	}
}

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
	// int threes;
	vector <vector <int> > colorDistance;

	state(){
		// threes = 0;
		depth = 0;
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
		vector <int> myVector(board_Size, -1);
		vector <vector <int> > tempVector(board_Size, myVector);
		int score = 0;
		int count = 0;
		int palindromeLength = 0;
		for (int row = 0; row < board_Size; row++){
			for (int col = 0; col < board_Size; col++){
				// color Distance Vector
				// count = 0;
				// while(1){
				// 	if(tempVector[board[row][col]][count] != -1){
				// 		tempVector[board[row][col]][count] = 10*row + col;
				// 		break;
				// 	}
				// 	count++;
				// }
				/*
				if(row == board_Size - 1){
					if(col == board_Size - 1){
						count = 0;
						while(1){
							if(tempVector[board[row][col+1]][count] != -1){
								tempVector[board[row][col+1]][count] = 10*row + col+1;
								break;
							}
							count++;
						}
						count = 0;
						while(1){
							if(tempVector[board[row+1][col+1]][count] != -1){
								tempVector[board[row+1][col+1]][count] = 10*(row+1) + col+1;
								break;
							}
							count++;
						}
					}
					count = 0;
					while(1){
						if(tempVector[board[row][col]][count] != -1){
							tempVector[board[row][col]][count] = 10*row + col;
							break;
						}
						count++;
					}
					count = 0;
					while(1){
						if(tempVector[board[row+1][col]][count] != -1){
							tempVector[board[row+1][col]][count] = 10*(row+1) + col;
							break;
						}
						count++;
					}
				}
				else{
					if(col == board_Size - 1){
						count = 0;
						while(1){
							if(tempVector[board[row][col+1]][count] != -1){
								tempVector[board[row][col+1]][count] = 10*row + col+1;
								break;
							}
							count++;
						}
					}
					count = 0;
					while(1){
						if(tempVector[board[row][col]][count] != -1){
							tempVector[board[row][col]][count] = 10*row + col;
							break;
						}
						count++;
					}
				}
				*/
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
		this->colorDistance = tempVector;
		this->stateScore = score;
		//return this->stateScore;
	}

	/*
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

	// void makeThrees(){
	// 	int current;
	// 	int next;
	// 	int length = 2;
	// 	for(int row = 0; row < board_Size-2; row++){
	// 		for(int col = 0; col < board_Size-2; col++){
	// 			current = this->board[row][col];
	// 			next = this->board[row][col+1];
	// 		}
	// 	}
	// }

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
