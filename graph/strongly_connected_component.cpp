
class SCC{
	int n;
	vector<vector<int> > G;
	vector<vector<int> > rG;
	
	void dfs(vector<bool> &visit, int pos, vector<int> &result){
		visit[pos] = true;
		for(int i=0; i<G[pos].size(); i++){
			if(visit[ G[pos][i] ]) continue;
			dfs(visit, G[pos][i], result);
		}
		result.push_back(pos);
	}

	void rdfs(vector<bool> &visit, int pos, int k){
		visit[pos] = true;
		component[pos] = k;
		for(int i=0; i<rG[pos].size(); i++){
			if(visit[ rG[pos][i] ]) continue;
			rdfs(visit, rG[pos][i], k);
		}
	}

	void make_rev(vector<vector<int> > &G){
		for(int i=0; i<n; i++){
			for(int j=0; j<G[i].size(); j++){
				rG[ G[i][j] ].push_back(i);
			}
		}
	}

public:
	int num_components;
	vector<int> component;
	vector< vector<int> > scc_graph;
	
	SCC(int n){
		this->n = n;
		G.resize(n);
		rG.resize(n);
	}

	SCC(vector<vector<int> > &G){
		this->n = G.size();
		this->G = G;
		rG.resize(n);
		make_rev(G);
	}
	
	void strongly_connected_components(){
		vector<bool> visit(n, false);
		vector<int> result;

		for(int i=0; i<n; i++){
			if(visit[i]) continue;
			dfs(visit, i, result);
		}

		
		component.resize(n);
		fill(visit.begin(), visit.end(), false);
		int k=0;
		for(int i=result.size()-1; i>=0; i--){
			if(visit[ result[i] ]) continue;
			rdfs(visit, result[i], k);
			k++;
		}

		num_components = k;
		scc_graph.resize( num_components );
		for(int i=0; i<n; i++){
			int cmp = component[i];
			for(int j=0; j<G[i].size(); j++){
				if(component[ G[i][j] ] == cmp) continue;
				scc_graph[cmp].push_back( component[ G[i][j] ] );
			}
		}
		
		for(int i=0; i<scc_graph.size(); i++){
			sort(scc_graph[i].begin(), scc_graph[i].end());
			scc_graph[i].erase( unique(scc_graph[i].begin(), scc_graph[i].end()), scc_graph[i].end());
		}
		
	}
	
	void add_edge(int from, int to){
		G[from].push_back(to);
		rG[to].push_back(from);
	}
};
