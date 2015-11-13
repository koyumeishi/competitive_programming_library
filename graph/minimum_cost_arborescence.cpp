#include <vector>

using namespace std;

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
	vector<int> component;	// node i is included in component[i]
	vector< vector<int> > scc_graph;	// new graph
	
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


struct edge{
	int from;
	int to;
	int cost;
	bool operator < (const edge& x) const{
		if(cost < x.cost) return true;
		return id < x.id;
	}
};

class MCA{
public:
	int cost;
	int V;
	vector<edge> E;
	MCA(int v) : cost(0), V(v) {

	}

	void rec( vector<vector<int>>& G_, vector<int>& E_ ){
		int c = 0;

		vector<vector<int>> G;
		vector<int> use(v, -1);
		for(int i=0; i<E_.size(); i++){
			if(use[ E[E_[i]].to ] < 0){
				use[ E[E_[i]].to ] = i;
			}else{
				if(E[E_[ use[ E[E_[i]].to ] ]].cost > E[E_[i]].to){
					use[ E[E_[i]].to ] = i;
				}
			}
		}

		SCC(G);
		SCC.strongly_connected_components();
		if(SCC.num_components == V){
			return c;
		}else{
			vector<int>& x = SCC.component;
			vector<int> component_size(SCC.num_components, 0);
			for(int i=0; i<x.size(); i++){
				component_size[ x[i] ]++;
			}
			vector<edge> E__;
			for(int i=0; i<E_.size(); i++){
				if(x[ E_[i].from ] == x[ E_[i].to ]) continue;
				E__.push_back((edge){ x[ E_[i].from ], x[ E_[i].to ], E_[i].cost });
				if(component_size[ x[ E_[i].to ] ] > 1){
					E__.back().cost -= E_[i].cost;
				}
			}

			rec(G__, E__);
		}


	}
};

#include <iostream>

int main(){
	int v,e,r;
	cin >> v >> e >> r;
	vector<edge> E;
	for(int i=0; i<e; i++){
		int s,t,w;
		cin >> s >> t >> w;
		E.push_back((edge){s,t,w});
	}
	sort(E.begin(), E.end());

	MCA(v);
	MCA.E = E;


}
