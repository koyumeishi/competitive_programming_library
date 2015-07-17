#include <vector>

using namespace std;

struct edge{
	int to;
	int id;
};

class Bridge{
	int size;
	vector<vector<edge>> G;
	vector<int> v;
	vector<bool> is_bridge;
	int num_bridge;
public:

	vector<pair<int,int>> bridge;
	vector<int> component;
	vector<vector<int>> new_Graph;

	//pre-calc
	void first_dfs(int pos, vector<bool>& visit, vector<bool>& used){
		if(visit[pos]){
			v[pos]--;
			return;
		}

		visit[pos] = true;
		for(int i=0; i<G[pos].size(); i++){
			edge& next = G[pos][i];
			if(used[next.id]) continue;
			if(visit[next.to]) v[pos]++;

			used[next.id] = true;
			first_dfs(next.to, visit, used);
		}
	}

	//imos finding-bridge
	int second_dfs(int pos, vector<bool>& visit){
		visit[pos] = true;

		int ret = v[pos];
		for(int i=0; i<G[pos].size(); i++){
			int next = G[pos][i].to;
			if(visit[next]) continue;

			int tmp = second_dfs(next, visit);
			ret += tmp;
			if(tmp == 0){
				is_bridge[G[pos][i].id] = true;
				num_bridge++;
				bridge.push_back({pos, next});
			}
		}

		return ret;
	}

	//get components that each node belong to
	void third_dfs(int pos, int c, vector<bool>& visit){
		visit[pos] = true;

		for(int i=0; i<G[pos].size(); i++){
			edge& next = G[pos][i];
			if(is_bridge[next.id]) continue;
			if(visit[next.to]) continue;
			third_dfs(next.to, c, visit);
		}

		component[pos] = c;
	}


	Bridge(vector<vector<edge>>& g, int e_size) : 
		G(g),
		size(g.size()),
		v(g.size()),
		is_bridge(e_size, false),
		num_bridge(0)
	{
		vector<bool> visit(size, false);
		vector<bool> used(e_size, false);

		for(int i=0; i<size; i++){
			if(visit[i]) continue;
			first_dfs(i, visit, used);
		}

		fill(visit.begin(), visit.end(), false);
		for(int i=0; i<size; i++){
			if(visit[i]) continue;
			second_dfs(i, visit);
		}


		component.resize(size, -1);
		
		fill(visit.begin(), visit.end(), false);
		int num_components = 0;
		for(int i=0; i<size; i++){
			if(visit[i]) continue;
			third_dfs(i, num_components, visit);
			num_components++;
		}

		//decomposited graph
		//this is a "forest"
		new_Graph.resize(num_components);
		for(int i=0; i<bridge.size(); i++){
			int u = bridge[i].first;
			int v = bridge[i].second;
			new_Graph[component[u]].push_back(component[v]);
			new_Graph[component[v]].push_back(component[u]);
		}
	}
};

#include <iostream>
int main(){
	int n,k;
	cin >> n >> k;
	vector<vector<edge>> G(n);
	for(int i=0; i<k; i++){
		int a,b;
		cin >> a >> b;
		G[a].push_back(edge{b,i});
		G[b].push_back(edge{a,i});
	}

	Bridge bridge(G, k);
	for(int i=0; i<bridge.bridge.size(); i++){
		cout << bridge.bridge[i].first << " --- " << bridge.bridge[i].second << endl;
	}
	return 0;
}