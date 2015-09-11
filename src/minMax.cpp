#include "gameTree2.cpp"

class minmax_Tree{
public:
    int gameDepth;
    int count;
    state* currentState;
    minmax_Tree(int depth){
        gameDepth=depth;
        count=0;
        currentState=new state();
    }

    bool terminal_Test(state* s){
        int sum=0;
        for(int i=0;i<board_Size;i++){
            sum+=s->colorCount[i];
        }
        if(sum==(board_Size*board_Size)){
            return true;
        }
        else if(s->depth>=gameDepth){
            return true;
        }
        return false;
    }
    action order_Decision(state* s,double alpha, double beta){
        action a;
        double value=-10000;
        vector<action> actions=s->orderActions();
        int l=actions.size();
        cout<<l<<endl;
        for(int i=0;i<l;i++){
            state *s1=s->result(s,actions.at(i));
            double val=min_Value(s1,alpha,beta);

            if(val>value){
                value=val;
                a=actions.at(i);
                count++;
            }
            if(alpha<val){
                alpha=val;
            }
            cout<<alpha<<" "<<val<<endl;
        }
        orderMove(a);
        return a;
    }
    action2 chaos_Decision(state* s,double alpha, double beta){
        action2 a;
        double value=10000;
        vector<action2> actions=s->chaosActions();
        int l=actions.size();
        for(int i=0;i<l;i++){
            double val=max_Value(s->result2(s,actions.at(i)),alpha,beta);
            if(val<value){
                value=val;
                a=actions.at(i);
                count++;
            }
            if(beta>val){
                beta=val;
            }
        }
        chaosMove(a);
        return a;
    }

    double min_Value(state* s,int alpha, int beta){
        if(terminal_Test(s)){
            double util=s->utility();
            delete s;
            return util;
        }
        double v=10000;
        vector<action2> actions=s->chaosActions();
        int l=actions.size();
        for(int i=0;i<l;i++){
            // cout<<actions.at(i).x<<" "<<actions.at(i).y<<" "<<actions.at(i).color<<endl;
            state *s1=s->result2(s,actions.at(i));
            double val=max_Value(s1,alpha,beta);
            if(val<v){
                v=val;
            }
            if(val<beta){
                beta=val;
            }
            if(beta<alpha){
                return val;
            }
            count++;
        }
        delete s;
        return v;
    }

    double max_Value(state* s,int alpha, int beta){
        if(terminal_Test(s)){
            double util=(s->probability)*s->utility();
            delete s;
            return util;
        }
        double v=-10000;
        vector<action> actions=s->orderActions();
        int l=actions.size();
        for(int i=0;i<l;i++){
            double val=min_Value(s->result(s,actions.at(i)),alpha,beta);
            if(val>v){
                v=val;
            }
            if(val>alpha){
                alpha=val;
            }
            if(beta<alpha){
                return val;
            }
            count++;
        }
        delete s;
        return v;
    }

    void chaosMove(action2 a){
        currentState->board[a.x][a.y]=a.color;
        currentState->colorCount[a.color]++;

    }
    void orderMove(action a){
        currentState->board[a.x1][a.y1]=currentState->board[a.x][a.y];
        currentState->board[a.x][a.y]=-1;
    }
};

int main(){
    board_Size=5;
	// minmax_Tree* minmax=new minmax_Tree(4);
	// cout<<minmax->currentState->palindromicScore();

	// minmax->order_Decision(minmax->currentState,-1000,1000);
	// for(int i=0;i<board_Size;i++){
	// 	cout<<minmax->currentState->colorCount[i];
	// 	cout<<endl;
	// }
	// cout<<endl<<minmax->count<<endl;


	// int r,b,y,o;
	// r=0;b=1;y=2;o=3;

	state s;

	s.board[0][0]=0;
	s.board[0][1]=0;
	s.board[0][2]=0;
	s.board[0][3]=0;
	s.board[0][4]=0;

	s.board[0][0]=0;
	s.board[1][0]=1;
	s.board[2][0]=2;
	s.board[3][0]=1;
	s.board[4][0]=0;
	s.board[4][2]=0;
	s.board[2][3]=2;
<<<<<<< HEAD
	s.board[4][3]=0;
=======
	s.board[4][3]=0;
>>>>>>> c391c52615cb6b1bcf4c1a16b4b162eb5548ffe8
	cout<<"score of state s = "<<s.utility()<<endl;
	 // action a(3,3,0,0);
	// state* s1=s.result(&s,a);
	// printvector(s.actions());
	// cout<<"partition"<<endl;
	// delete s1;
	//printvector(s1->actions());

	//cout<<s.board[3][3];

	// cout<<a.x<<a.y<<a.x1<<a.y1;
}
