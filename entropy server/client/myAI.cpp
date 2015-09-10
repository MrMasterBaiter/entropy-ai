// sample clint which plays randomly c++

#include "stdc++.h"
// #include <iostream>
#define FOR(i, n) for(int i=0; i<n; i++)
using namespace std;

int N;
char board[5][5];

void init() {
	FOR(i, N){
		FOR(j, N){
			board[i][j] = '-';
		}
	}
}

void playAsOrder() {
	int x, y;	char c;
	FOR(i, N*N-1) {
		cin>>x>>y>>c;
		cout<<x<<' '<<y<<' '<<x<<' '<<y<<endl;
	}
}

void playAsChaos() {
	int a, b, c, d; char color;
	cin>>color;
	cout<<"0 0"<<endl;	// First move :D
	board[0][0] = color;
	FOR(i, N*N-1) {
		cin>>a>>b>>c>>d;
		cin>>color;
		if (a != c || b != d) {
			board[c][d] = board[a][b];
			board[a][b] = '-';
		}
		
		FOR(ii, N) {
			FOR(jj, N) {
				if (board[ii][jj] == '-') {
					board[ii][jj] = color;
					cout<<ii<<' '<<jj<<endl;
					ii = N; jj = N;
				}
			}
		}
	}
	cin>>a>>b>>c>>d;
}

int main() {
	cin>>N;	init();
	string s;	cin>>s;
	
	if (s.compare("ORDER") == 0) {
		playAsOrder();
	} else {
		playAsChaos();
	}
	return 0;
}