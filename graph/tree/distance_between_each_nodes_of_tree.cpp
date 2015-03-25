#include <vector>
#include <map>

//O(N^2)
//for tree

typedef struct{
	int to;
	int dist;
}edge;

//O(N)
void dfs(vector<vector<edge> > &G, int pos, int last, vector<long long> &d){
	if(G[pos].size() == 1 && G[pos][0].to == last){
		return;
	}

	for(int i=0; i<G[pos].size(); i++){
		if(G[pos][i].to == last) continue;
		d[ G[pos][i].to ] = d[pos] + G[pos][i].cost;
		dfs(G, G[pos][i].to, pos, d);
	}
}

vector<vector<long long> > dist_between_each_nodes(vector<vector<edge> > &G){
	vector<vector<long long> > ret(G.size());
	for(int i=0; i<G.size(); i++){
		ret[i] = vector<long long>(G[i].size(), 0);
		dfs_dist_between_each_nodes(G, i, -1, ret[i]);
	}
	return ret;
}
