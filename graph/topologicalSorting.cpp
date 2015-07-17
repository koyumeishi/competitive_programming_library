#include <vector>
#include <set>

//dfs
//O(V+E)
void TopologicalSort(vector<vector<int> > &G, vector<int> &res, int node, vector<bool> &visit){
	if(visit[node] == true) return;
	visit[node] = true;
	for(auto itr = G[node].rbegin(); itr != G[node].rend(); itr++){
		TopologicalSort(G, res, *itr, visit);
	}
	/*
	for(int i=0; i<G[node].size(); i++){
		TopologicalSort(G, res, G[node][i], visit);
	}
	*/
	res.push_back(node);
}

//lexicographically smallest
// O(N logN)
//vector<vector<int> > &G is edge list. G[i][j] := edge goes to node[G[i][j]] from node[i] 
//vector<set<int> > from . from[i] := nodes set which have edge going to node[i]
//set<int> begin := the nodes those indegree is zero
vector<int> TopologicalSorting(vector<vector<int> > &G, vector< set<int> > &from, set<int> &begin){
	vector<int> ret;
	while( begin.size() > 0 ){
		int val = *(begin.begin());	// using "begin.rbegin()", lexicographically largest one
		begin.erase( val );
		ret.push_back(val);
		for(auto itr = G[val].begin() ; itr != G[val].end(); itr++){
			int m = *itr;
			if( from[m].size() == 1 ){
				begin.insert(m);
			}
			from[m].erase(val);
		}
	}
	return ret;
}