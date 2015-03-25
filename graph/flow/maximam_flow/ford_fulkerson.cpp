#include <vector>
#include <iostream>


using namespace std;

struct edge{
	int to;
	int cap;
	int rev;
};

int Ford_Fulkerson_dfs(vector<vector<edge> > &G, int pos, int flow, vector<bool> &used, int t){
	if(pos == t){
		return flow;
	}else{
		used[pos] = true;
		for(int i=0; i<G[pos].size(); i++){
			if(!used[G[pos][i].to] && G[pos][i].cap >0){
				int myflow = Ford_Fulkerson_dfs(G, G[pos][i].to, min(flow, G[pos][i].cap), used, t);
				if(myflow > 0){
					G[pos][i].cap -= myflow;
					G[ G[pos][i].to ][ G[pos][i].rev ].cap += myflow;
					return myflow;
				}
			}
		}
		return 0;
	}
}

int Ford_Fulkerson(vector<vector<edge> > &G, int s, int t){
	const int INF = 100000000;
	int flow=0;
	for(;;){
		vector<bool> used(G.size(), false);
		int f = Ford_Fulkerson_dfs(G, s, INF, used, t);
		if(f==0) break;
		else flow += f;
	}
	return flow;
}

void add_edge(vector<vector<edge> > &G, int from, int to, int cap){
	G[from].push_back( (edge){to, cap, (int)G[to].size()} );
	G[to].push_back( (edge){from, 0, (int)G[from].size()-1} );
}

int main(){
	int n;
	cin >> n;
	
	int s,t;
	cin >> s >> t;
	
	vector<vector<edge> > G(n);
	int e;
	cin >> e;
	for(int i=0; i<e; i++){
		int from, to, cap;
		cin >> from >> to >> cap;
		add_edge(G,from,to,cap);
	}
	cout << Ford_Fulkerson(G, s,t) << endl;
	
	return 0;
}