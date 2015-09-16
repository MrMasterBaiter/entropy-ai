#include "gameTree.cpp"
class position{
public:
	int x,y;
	position(){
		x=-1;
		y=-1;
	}
	position(int a,int b){
		x=a;
		y=b;
	}
};
class colorNode{
public:
	int color;
	double probability;
	colorNode(int c,double p){
		color=c;
		probability=p;
	}
};
class actionValuePair{
public:
	double value;
	action act;
	actionValuePair(){
	}
	actionValuePair(double v,action b){
		value=v;
		act=b;
	}
};
class action2ValuePair{
public:
	double value;
	action2 act;
	action2ValuePair(){
	
	}
	action2ValuePair(double v,action2 b){
		value=v;
		act=b;
	}
};

class minmax_Tree{
public:
bool alternate;
state* currentState;
int gameDepth;
int *color;
int *colorCount;
position** colorPositions;
int count;
double* chance;
int nodecount;

void initColorPositions(){
	colorPositions=new position*[board_Size];
	for(int i=0;i<board_Size;i++){
		colorPositions[i]=new position[board_Size];
	}
}

minmax_Tree(int depth){
	bool b=false;
	alternate=false;
	color=new int [board_Size];
	colorCount=new int[board_Size];
	currentState=new state();
	chance=new double[depth+1];
	gameDepth=depth;
	count=0;
	for(int i=0;i<board_Size;i++){
		color[i]=i;
		colorCount[i]=0;
	}
	for(int i=0;i<(depth+1);i++){
		chance[i]=1;
	}
	initColorPositions();
	nodecount=0;
}
bool terminal_Test(){
	if(count==(board_Size*board_Size)){
		return true;
	}
	else if(currentState->depth>=gameDepth){
		return true;
	}
	return false;
}

action order_Decision(double alpha, double beta){
	actionValuePair av;
	if(count==1||count==2){
		action a(0,0,0,0);
		av.act=a;
		av.value=0;
	}
	else{
		av= max_Value(alpha,beta);
	}
	orderFinalMove(av.act);
	return av.act;
}
action2 chaos_Decision(int clr,double alpha, double beta){
	action2ValuePair a2v;
	if(count==0||count==1||count==2){
		action2 a;
		if(currentState->board[0][0]==-1){
			action2 b(0,0,clr,1);
			a=b;
		}
		else if(currentState->board[board_Size-1][board_Size-1]==-1){
			action2 b(board_Size-1,board_Size-1,clr,1);
			a=b;
		}
		else{
			action2 b(board_Size-1,0,clr,1);
			a=b;
		}
		a2v.act=a;
		a2v.value=0;
	}
	else{
		a2v= min_Value(clr,alpha,beta);
	}
	chaosFinalMove(a2v.act);
	return a2v.act;
}
actionValuePair max_Value(double alpha, double beta){
	action a2;
	if(terminal_Test()){
		double util=currentState->utility(count);
		int d=currentState->depth;
		actionValuePair av(chance[d]*util,a2);
		return av;
	}
	double v=-10000;
	vector<action> actions=orderActions();
	int l=actions.size();
	for(int i=0;i<l;i++){
		nodecount++;
		action a=actions.at(i);
		orderMove(a);//depth++
		double val=min_Value(-1,alpha,beta).value;
		orderDeMove(a);//depth--

		if(v<val){
			v=val;
			a2=a;
		}
		if(v>beta){
			actionValuePair av(v,a2);
			return av;
		}
		if(alpha<v){
			alpha=v;
		}
	}
	actionValuePair av(v,a2);
	return av;
}
action2ValuePair min_Value(int clr,int alpha, int beta){
	action2 a2;
	if(terminal_Test()){
		double util=currentState->utility(count);
		int d=currentState->depth;
		action2ValuePair a2v(chance[d]*util,a2);
		return a2v;
	}
	double v=10000;
	vector<colorNode> cn=generateColors(clr);
	vector<action2> actions=chaosActions(cn);
	int l=actions.size();
	for(int i=0;i<l;i++){
		nodecount++;
		action2 a=actions.at(i);
		chaosMove(a);
		double val=max_Value(alpha,beta).value;
		chaosDeMove(a);
		if(v>val){
			v=val;
			a2=a;
		}
		if(v<alpha){
			action2ValuePair a2v(v,a2);
			return a2v;
		}
		if(beta>v){
			beta=v;
		}
	}
	action2ValuePair a2v(v,a2);
	return a2v;
}
vector<colorNode> generateColors(int clr){
	vector<colorNode> colornodes;
	if(currentState->depth==0){
		colorNode cn(clr,1);
		colornodes.push_back(cn);
		return colornodes;
	}
	for(int k=0;k<board_Size;k++){
		if(colorCount[k]<board_Size){
			double y=(board_Size*board_Size-count);
			double x=(board_Size-colorCount[k]);
			double p=x/y;
			colorNode cn(color[k],p);
			colornodes.push_back(cn);
		}
		else{
			break;
		}
	}
	return colornodes;
}

vector<action2> chaosActions(vector<colorNode> cn){
	vector<action2> actions;
	int l=cn.size();
	for(int i=0;i<board_Size;i++){
		for(int j=0;j<board_Size;j++){
			if(currentState->board[i][j]==-1){
				for(int k=0;k<l;k++){
					colorNode node=cn.at(k);
					action2 a(i,j,node.color,node.probability);
					actions.push_back(a);
				}
				
			}
		}
	}
		
	return actions;
}
vector<action> orderActions(){
	vector<action> actions;
	bool b=false;
	for(int i=0;i<board_Size;i++){
		for(int j=0;j<board_Size;j++){
			if(currentState->board[i][j]!=-1){
				vector<action> right,left,up,down;
				right=moveRight(i,j);
				left=moveLeft(i,j);
				up=moveUp(i,j);
				down=moveDown(i,j);
				actions.insert(actions.end(),right.begin(),right.end());
				actions.insert(actions.end(),left.begin(),left.end());
				actions.insert(actions.end(),up.begin(),up.end());
				actions.insert(actions.end(),down.begin(),down.end());
				if(!b){
					b=true;
					action a(i,j,i,j);
					actions.push_back(a);
				}
			}
		}
	}
	return actions;
}
vector<action> moveRight(int i,int j){
	int k=i+1;
	vector<action> actions;
	while(k<board_Size){
		if(currentState-> board[k][j]==-1){
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
		if(currentState-> board[k][j]==-1){
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
		if(currentState->board[i][k]==-1){
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
		if(currentState->board[i][k]==-1){
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

void chaosFinalMove(action2 a){
	currentState->board[a.x][a.y]=a.color;
	addColor(a.color);
}
void chaosMove(action2 a){
	currentState->board[a.x][a.y]=a.color;
	addColor(a.color);
	currentState->depth++;
	int x=currentState->depth;
	chance[x]=chance[x-1]*a.probability;
}
void chaosDeMove(action2 a){
	currentState->board[a.x][a.y]=-1;
	removeColor(a.color);
	currentState->depth--;

}
void orderFinalMove(action a){
	int temp=currentState->board[a.x][a.y];
	currentState->board[a.x][a.y]=currentState->board[a.x1][a.y1];
	currentState->board[a.x1][a.y1]=temp;	
}

void orderMove(action a){
	int temp=currentState->board[a.x][a.y];
	currentState->board[a.x][a.y]=currentState->board[a.x1][a.y1];
	currentState->board[a.x1][a.y1]=temp;	
	currentState->depth++;
	int x=currentState->depth;
	chance[x]=chance[x-1];
}
void orderDeMove(action a){
	action b(a.x1,a.y1,a.x,a.y);
	int temp=currentState->board[b.x][b.y];
	currentState->board[b.x][b.y]=currentState->board[b.x1][b.y1];
	currentState->board[b.x1][b.y1]=temp;	
	currentState->depth--;
	
}
void addColor(int clr){
	count++;
	for(int i=0;i<board_Size;i++){
		if(color[i]==clr){
			colorCount[i]++;
			for(int j=i+1;j<board_Size;j++){
				if(colorCount[j-1]>colorCount[j]){
					int temp=colorCount[j-1];
					colorCount[j-1]=colorCount[j];
					colorCount[j]=temp;

					temp=color[j-1];
					color[j-1]=color[j];
					color[j]=temp;
				}
				else {
					break;
				}
			}
			break;
		}
	}
}
void removeColor(int clr){
	count--;
	for(int i=0;i<board_Size;i++){
		if(color[i]==clr){
			colorCount[i]--;
			for(int j=i-1;j>=0;j--){
				if(colorCount[j]>colorCount[j+1]){
					int temp=colorCount[j];
					colorCount[j]=colorCount[j+1];
					colorCount[j+1]=temp;

					temp=color[j];
					color[j]=color[j+1];
					color[j+1]=temp;
				}
				else {
					break;
				}
			}
			break;
		}
	}
}

};
void moveOrder(){
	minmax_Tree* minmax = new minmax_Tree(4);
	int x,y;
	char c;
	int color;
	for(int moves = 0; moves < 25; moves++){
		// cerr<<"start input for order"<<endl;

		cin>>x;
		cin>>y;
		cin>>c;
		// cerr<<"color "<<c<<endl;
		
		color = c;
		color -= 65;
		action2 a(x, y, color, 1);
		minmax->chaosFinalMove(a);
		cerr<<"score of state is "<<minmax->currentState->utility(minmax->count)<<endl;
		if(moves<24){
			action b = minmax->order_Decision(-1000, 1000);
			cout << b.x << " " << b.y << " " << b.x1 << " " << b.y1 << endl;
			cerr << b.x << " " << b.y << " " << b.x1 << " " << b.y1 << endl;
		}
				
	}
	delete[] minmax;
}

/* TO DO */
void moveChaos(){
	minmax_Tree* minmax = new minmax_Tree(4);
	int i = 0;
	char c;
	int color;
	int x, y, x1, y1;
	for(; i < 25; i++){
		if(i == 0){
			cin>>c;
			color = c;
			color -= 65;
			action2 b = minmax->chaos_Decision(color,-1000, 1000);
			cout << b.x << " " << b.y << endl;
		}
		else{
			cin >> x >> y >> x1 >> y1;
			action a(x, y, x1, y1);
			minmax->orderFinalMove(a);
			cin>>c;
			color = c;
			color -= 65;
			action2 b = minmax->chaos_Decision(color,-1000, 1000);
			cout << b.x << " " << b.y << endl;
		}
	}
	delete[] minmax;
}

int main(){
	// cout<<"minmax tree created"<<endl;
	cin>>board_Size;
	string role;
	cin>>role;
	// cout<<"size "<<size<<" role "<<role<<endl;
	if(role == "ORDER"){
		cerr<<"playing as ORDER"<<endl;
		moveOrder();
		moveChaos();
	}
	else{
		cerr<<"playing as CHAOS"<<endl;
		moveChaos();
		moveOrder();
	}
    return 0;

	
}

