#include <vector>
#include <algorithm>

using namespace std;
 
//G : adjacency matrix
//CAUTION: initialize G[i][i] as 0 , and do G[i][j] as INF if there are no edges between i and j
//O(V^3)
void Warshall_Floyd(vector<vector<int>> &G, vector<vector<int>> &dist){
	//const int INF = 100000000;
	int n=G.size();

	dist = G;

	for(int k=0; k<n; k++){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
}




typedef struct{
	int to;
	int cost;
}edge;

//G : adjacency list
//O(V^3)
void Warshall_Floyd(vector<vector<edge>> &G, vector<vector<int>> &dist){
	const int INF = 100000000;
	int n=G.size();

	for(auto &v: dist) fill(v.begin(), v.end(), INF);

	for(int i=0; i<n; i++){
		dist[i][i] = 0;

		for(int j=0; j<G[i].size(); j++){
			dist[i][ G[i][j].to ] = G[i][j].cost;
		}
	}

	for(int k=0; k<n; k++){
		for(int i=0; i<n; i++){
			for(int j=0; j<n; j++){
				dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
}
