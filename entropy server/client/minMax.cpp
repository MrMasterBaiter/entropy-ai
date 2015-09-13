#include "gameTree.cpp"

class minmax_Tree{
public:
	state* currentState;
	int gameDepth;
	int *color;
	int *colorCount;
	int count;
	double* chance;
	int nodecount;
	int nodecount2;

	minmax_Tree(int depth){
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
		nodecount2=nodecount=0;
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

	action order_Decision(double alpa, double bta){
		double alpha=alpa;
		double beta=bta;
		action a;
		double value=-10000;
		vector<action> actions=currentState->orderActions();
		int l=actions.size();
		nodecount2+=l;
		nodecount+=l;
		for(int i=0;i<l;i++){
			action b=actions.at(i);
			orderMove(b);
			// cout<<alpha<<" "<<beta<<endl;
			double val=min_Value(alpha,beta);
			orderDeMove(b);
			if(val>value){
				value=val;
				a=b;
			}
			if(alpha<val){
				alpha=val;
			}
		}
		orderFinalMove(a);
		return a;
	}
	action2 chaos_Decision(double alpa, double bta){
		double alpha=alpa;
		double beta=bta;
		action2 a;
		double value=10000;
		vector<action2> actions=chaosActions();
		int l=actions.size();
		for(int i=0;i<l;i++){
			action2 b=actions.at(i);
			chaosMove(b);
			double val=max_Value(alpha,beta);
			chaosDeMove(b);
			if(val<value){
				value=val;
				a=b;
			}
			if(beta>val){
				beta=val;
			}
		}
		chaosFinalMove(a);
		return a;
	}
	double max_Value(double alpa, double bta){
		double alpha=alpa;
		double beta=bta;
		if(terminal_Test()){
			double util=currentState->utility();
			int d=currentState->depth;
			return chance[d]*util;
		}
		double v=-10000;
		vector<action> actions=currentState->orderActions();
		int l=actions.size();
		nodecount2+=l;
		for(int i=0;i<l;i++){
			nodecount++;
			action a=actions.at(i);
			orderMove(a);
			double val=min_Value(alpha,beta);
			orderDeMove(a);

			if(val>v){
				v=val;
			}
			if(val>alpha){
				alpha=val;
			}
			if(beta<alpha){
				return val;
			}
		}
		return v;
	}
	double min_Value(int alpa, int bta){
		double alpha=alpa;
		double beta=bta;
		if(terminal_Test()){
			double util=currentState->utility();
			int d=currentState->depth;
			return chance[d]*util;
		}
		double v=10000;
		vector<action2> actions=chaosActions();
		int l=actions.size();
		nodecount2+=l;
		for(int i=0;i<l;i++){
			nodecount++;
			action2 a=actions.at(i);
			chaosMove(a);
			double val=max_Value(alpha,beta);
			chaosDeMove(a);
			if(val<v){
				v=val;
			}
			if(val<beta){
				beta=val;
			}
			if(beta<alpha){
				return val;
			}
		}
		return v;
	}
	vector<action2> chaosActions(){
		vector<action2> actions;
		for(int i=0;i<board_Size;i++){
			for(int j=0;j<board_Size;j++){
				if(currentState->board[i][j]==-1){
					for(int k=0;k<board_Size;k++){
						if(colorCount[k]<board_Size){
							double y=(board_Size*board_Size-count);
							double x=(board_Size-count);
							double p=x/y;
							action2 a(i,j,color[k],p);
							actions.push_back(a);
						}
						else{
							break;
						}
					}
				}
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
		action b = minmax->order_Decision(-1000, 1000);
		// cerr<<"after action"<<endl;
		// minmax->currentState->printboard();
		cerr<<"score of state is "<<minmax->currentState->utility()<<endl;
		cout << b.x << " " << b.y << " " << b.x1 << " " << b.y1 << endl;
		cerr << b.x << " " << b.y << " " << b.x1 << " " << b.y1 << endl;
		// cout<<minmax->nodecount<<" "<<minmax->nodecount2<<endl;
		// minmax->nodecount=minmax->nodecount2=0;
		// cout<<minmax->nodecount<<" "<<minmax->nodecount2<<endl;
		// minmax->nodecount=0;
		// minmax->nodecount2=0;
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
		// first move
		if(i == 0){
			cin>>c;
			color = c;
			color -= 65;
			action2 b = minmax->chaos_Decision(-1000, 1000);
			cout << b.x << " " << b.y << endl;
		}
		else{
			cin >> x >> y >> x1 >> y1;
			action a(x, y, x1, y1);
			minmax->orderFinalMove(a);
			action2 b = minmax->chaos_Decision(-1000, 1000);
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
// action order_Decision(state* s,double alpha, double beta){
// 	int besti=board_Size;
// 	action a;
// 	double value=-10000;
// 	vector<action> actions=s->orderActions();
// 	int l=actions.size();
// 	nodecount+=l;
// 	nodecount2+=l;
// 	for(int i=0;i<l;i++){
// 		state *s1=s->result(s,actions.at(i));
// 		double val=min_Value(s1,alpha,beta,besti);
// 		if(val>value){
// 			value=val;
// 			a=actions.at(i);
// 		}
// 		if(alpha<val){
// 			alpha=val;
// 		}
// 	}
// 	orderMove(a);
// 	return a;
// }

// action2 chaos_Decision(state* s,double alpha, double beta){
// 	action2 a;
// 	int besti=board_Size;
// 	double value=10000;
// 	vector<action2> actions;
// 	int colorLeft=(board_Size*board_Size)-count;
// 	for(int i=0;i<besti;i++){
// 		if(colorCount[i]<board_Size){
// 			double p=(board_Size- colorCount[i])/colorLeft;
// 			vector<action2> a2=s->chaosActions(color[i],p);
// 			actions.insert(actions.end(),a2.begin(),a2.end());
// 		}

// 	}
// 	int l=actions.size();
// 	nodecount+=l;
// 	nodecount2+=l;
// 	for(int i=0;i<l;i++){
// 		state* s1=s->result2(s,actions.at(i));
// 		double val=max_Value(s1,alpha,beta);
// 		if(val<value){
// 			value=val;
// 			a=actions.at(i);
// 		}
// 		if(beta>val){
// 			beta=val;
// 		}
// 	}
// 	chaosMove(a);
// 	return a;
// }

// double min_Value(state* s,int alpha, int beta,int  besti){
// 	if(terminal_Test(s)){
// 		double util=(s->probability)*(s->utility());
// 		delete s;
// 		return util;
// 	}
// 	double v=10000;
// 	int colorLeft=(board_Size*board_Size)-count;
// 	vector<action2> actions;
// 	for(int i=0;i<besti;i++){
// 		if(colorCount[i]<board_Size){
// 			double p=(board_Size- colorCount[i])/colorLeft;
// 			vector<action2> a=s->chaosActions(color[i],p);
// 			actions.insert(actions.end(),a.begin(),a.end());
// 		}

// 	}
// 	int l=actions.size();
// 	nodecount2+=l;
// 	for(int i=0;i<l;i++){
// 		nodecount++;
// 		action2 action=actions.at(i);
// 		addColor(action.color);
// 		state *s1=s->result2(s,action);
// 		double val=max_Value(s1,alpha,beta);
// 		removeColor(action.color);
// 		if(val<v){
// 			v=val;
// 		}
// 		if(val<beta){
// 			beta=val;
// 		}
// 		if(beta<alpha){
// 			return val;
// 		}
// 	}
// 	delete s;
// 	return v;
// }

// double max_Value(state* s,int alpha, int beta){
// 	int besti=board_Size;
// 	if(terminal_Test(s)){
// 		double util=(s->probability)*s->utility();
// 		delete s;
// 		return util;
// 	}
// 	double v=-10000;
// 	vector<action> actions=s->orderActions();
// 	int l=actions.size();
// 	nodecount2+=l;
// 	for(int i=0;i<l;i++){
// 		nodecount++;
// 		state* s1=s->result(s,actions.at(i));
// 		double val=min_Value(s1,alpha,beta,besti);
// 		if(val>v){
// 			v=val;
// 		}
// 		if(val>alpha){
// 			alpha=val;
// 		}
// 		if(beta<alpha){
// 			return val;
// 		}
// 	}
// 	delete s;
// 	return v;
// }
