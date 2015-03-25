#include <iostream>
#include <vector>
#include <queue>

using namespace std;
#define INF 1e9

typedef struct{
	int to;
	int cap;
	int cost;
	int rev;
}edge;

//min cost : s->t (flow:f)
int min_cost_flow(vector<vector<edge> > &G, int s, int t, int f){
	int cost = 0;
	vector<int> prev_v(G.size(),-1);
	vector<int> prev_e(G.size(),-1);
	
	while(f>0){
		//min distance(cost based) search with Belman-Ford
		vector<int> dist(G.size(), INF);
		dist[s] = 0;
		prev_v[s] = s;
		bool update=true;
		while(update){
			update = false;
			for(int i=0; i<G.size(); i++){
				if(dist[i] >= INF) continue;
				
				vector<edge> &e = G[i];
				for(int j=0; j<e.size(); j++){
					if(dist[e[j].to] > dist[i] + e[j].cost && e[j].cap>0){
						dist[e[j].to] = dist[i] + e[j].cost;
						update = true;
						prev_v[ e[j].to ] = i;
						prev_e[ e[j].to ] = j;
					}
				}
			}
		}

		//cannot achieved to "t" return -1
		if(dist[t]>=INF) return -1;

		
		//add cost of s->t with flow=d
		int pos=t;
		int d=INF;
		while(pos!=s){
			int i=prev_v[pos];
			int j=prev_e[pos];
			pos = i;
			d = min(d, G[i][j].cap);
		}
		
		pos = t;
		//cout << t ;
		while(pos!=s){
			int i=prev_v[pos];
			int j=prev_e[pos];
			G[i][j].cap -= d;
			G[ G[i][j].to ][ G[i][j].rev ].cap += d;
			//cost += G[i][j].cost * d;
			pos = i;
			//cout << " <- " << pos;
		}
		//cout << endl;
		cost += d * dist[t];
		f -= d;

		//f==0 then end
	}
	return cost;
}


void add_edge(vector<vector<edge> > &G, int from, int to, int cap, int cost){
	G[from].push_back((edge){to, cap, cost, G[to].size()});
	G[to].push_back((edge){from, 0, -cost, (int)G[from].size()-1});
}
