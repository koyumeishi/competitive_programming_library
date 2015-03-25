#include <vector>
#include <iostream>
#include <queue>
#include <algorithm>

using namespace std;

typedef struct{
	int to;
	int cap;
	int rev;
}edge;

// node v : distance from s => level[v]
void bfs(vector<vector<edge> > &G, vector<int> &level, int s){
	fill(level.begin(), level.end(), -1);
	queue<int> q;
	q.push(s);
	level[s] = 0;
	while(!q.empty()){
		int e=q.front(); q.pop();
		for(int i=0; i<G[e].size(); i++){
			if(G[e][i].cap > 0 && level[G[e][i].to] < 0){
				level[G[e][i].to] = level[e] + 1;
				q.push(G[e][i].to);
			}
		}
	}
}

int dfs(vector<vector<edge> > &G, vector<int> &level, vector<bool> &used, vector<int> &iter, int s, int f,  int t){
	if(s==t) return f;
	else{
		//iter[e] : done searching from v[0] to v[ iter[e]-1 ]
		for(int &i=iter[s]; i<G[s].size(); i++){
			//distance from s to v[e][i].to must be longer than dist from s to v
			if(G[s][i].cap > 0 && level[s] < level[ G[s][i].to ]){
				int d = dfs(G, level, used, iter, G[s][i].to, min(f, G[s][i].cap), t);
				if(d>0){
					G[s][i].cap -= d;
					G[ G[s][i].to ][ G[s][i].rev ].cap += d;
					return d;
				}
			}
		}
		return 0;
	}
}

int dinic_maxflow(vector<vector<edge> > &G, int s, int t){
	const int INF = 100000000;
	int flow=0;
	while(true){
		vector<int> level(G.size(), -1);
		vector<int> iter(G.size(), 0);
		vector<bool> used(G.size(), false);
		bfs(G, level, s);
		if(level[t] < 0) return flow;	//unable to achieve to t
		while(true){
			int f = dfs(G, level, used, iter, s, INF, t);
			if(f==0) break;
			else flow += f;
		}
	}
}

void add_edge(vector<vector<edge> > &G, int from, int to, int cap){
	G[from].push_back((edge){to, cap, (int)G[to].size()});
	G[to].push_back((edge){from, 0, (int)G[from].size() - 1});
}

int main(){


	return 0;
}