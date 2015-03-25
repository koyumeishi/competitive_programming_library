#include <vector>
#include <map>

//O(2*N)

typedef struct{
	int to;
	int dist;
}edge;

pair<long long, int> dfs_diameter_of_tree(vector<vector<edge> > &G, int pos, int last, long long d){
	pair<long long, int> ret = make_pair( d, pos );
	
	if(G[pos].size() == 1 && G[pos][0].to == last){
		return ret;
	}

	for(int i=0; i<G[pos].size(); i++){
		if(G[pos][i].to == last) continue;
		ret = max( ret, dfs_diameter_of_tree(G, G[pos][i].to, pos, d + G[pos][i].cost) );
	}
	return ret;
}

int diameter_of_tree(vector<vector<edge> > &G){
	pair<long long, int> d1 = dfs_diameter_of_tree(G, 0, -1, 0);
	pair<long long, int> d2 = dfs_diameter_of_tree(G, d1.second, -1, 0);
	return d2.second;
}
