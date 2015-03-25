#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>

using namespace std;
 
typedef struct{
	int to;
	int cost;
}edge;
 
 
 
//distance from s
//O(E log V)
void dijkstra(vector<vector<edge> > &G, vector<int> &dist, int s){

	//INF as distance
	const int INF = 10000000;
	
	//first : distance from s, second : its vertex
	priority_queue< pair<int,int> , vector<pair<int,int> >, greater< pair<int,int> > > pq;
	 
	fill(dist.begin(), dist.end(), INF);
	dist[s] = 0;
	pq.push( make_pair(dist[s], s) );
	while(!pq.empty()){
		pair<int,int> q = pq.top();
		pq.pop();
		int from = q.second;
		if(dist[from] < q.first) continue; // it's not minimum distance
		int n=G[from].size();
		for(int i=0; i<n; i++){
			edge e = G[from][i];
			if(dist[e.to] > dist[from] + e.cost){
				dist[e.to] = dist[from] + e.cost;
				pq.push( make_pair(dist[e.to], e.to) );
			}
		}
	}
}
 
void add_edge(vector<vector<edge> > &G, int from, int to, int cost){
	G[from].push_back( (edge){to, cost} );
	G[to].push_back( (edge){from, cost} );
}
 