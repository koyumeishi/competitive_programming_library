#include <vector>
#include <set>

//dfs
//O(V+E)
{
	void TopologicalSort_dfs(vector<vector<int> > &G, vector<int> &res, int node, vector<bool> &visit){
		if(visit[node] == true) return;
		visit[node] = true;
		for(auto itr = G[node].rbegin(); itr != G[node].rend(); itr++){
			TopologicalSort_dfs(G, res, *itr, visit);
		}
		/*
		for(int i=0; i<G[node].size(); i++){
			TopologicalSort(G, res, G[node][i], visit);
		}
		*/
		res.push_back(node);
	}

	vector<int> TopologicalSort(vector<vector<int>> &G){
		int n = G.size();
		vector<int> ret;
		vector<bool> visit(n,false);
		for(int i=0; i<n; i++){
			if(visit[i]) continue;
			TopologicalSort_dfs(G, ret, i, visit);		
		}
		reverse(ret.begin(), ret.end());
		return ret;
	}
}

//lexicographically smallest
// O(N logN)
//vector<vector<int> > &G is edge list. G[i][j] := edge goes to node[G[i][j]] from node[i] 
//vector<set<int> > from . from[i] := nodes set which have edge going to node[i]
//set<int> begin := the nodes those indegree is zero
{
	vector<int> TopologicalSorting_lex_smallest(vector<vector<int> > &G, vector< set<int> > &from, set<int> &begin){
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

	vector<int> TopologicalSorting_lex_smallest_call(vector<vector<int>> &G){
		int n = G.size();
		vector<set<int>> rev(n);
		set<int> begin;
		for(int i=0; i<n; i++){
			for(int e: G[i]){
				rev[e].insert(i);
			}
		}
		for(int i=0; i<n; i++){
			if(rev[i].size() == 0){
				begin.insert(i);
			}
		}

		return TopologicalSorting_lex_smallest(G, rev, begin);
	}
}