#include <iostream>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

struct node{
	int par;
	vector<int> ch;
};

class HL_Decomposition{
public:

	vector<vector<int>> decomposited_tree;
	vector<pair<int,int>> node_pos;

	HL_Decomposition(vector<vector<int>> Tree){
		int n = Tree.size();
		vector<int> sub_tree_size(n, 0);
		vector<bool> visit(n, false);

		function<int(int)> dfs_count = [&](int pos) -> int{
			if(visit[pos]) return 0;
			visit[pos] = true;
			int ret = 1;
			for(int next: Tree[pos]){
				ret += dfs_count(next);
			}

			sub_tree_size[pos] = ret;
			return ret;
		};

		int root = 0;

		dfs_count(0);

		node_pos.resize(n);

		fill(visit.begin(), visit.end(), false);
		queue<int> q;
		q.push(root);
		while(!q.empty()){
			int pos = q.front(); q.pop();
			if(visit[pos]) continue;
			visit[pos] = true;

			decomposited_tree.push_back({pos});
			node_pos[pos] = {decomposited_tree.size()-1, 0};

			while(true){
				int max_size = 0;
				int next = -1;
				for(int i=0; i<Tree[pos].size(); i++){
					if(visit[Tree[pos][i]]) continue;
					if(max_size < sub_tree_size[Tree[pos][i]]){
						
						if(next>=0) q.push(next);

						max_size = sub_tree_size[Tree[pos][i]];
						next = Tree[pos][i];
					}else{
						q.push(Tree[pos][i]);
					}
				}

				if(next < 0) break;
				decomposited_tree.back().push_back(next);
				node_pos[next] = {decomposited_tree.size()-1, decomposited_tree.back().size()-1};
				visit[next] = true;
				pos = next;
			}
		}

	}

	HL_Decomposition(vector<node> Tree){
		int n = Tree.size();
		vector<int> sub_tree_size(n, 0);

		function<int(int)> dfs_count = [&](int pos) -> int{
			int ret = 1;
			for(int next: Tree[pos].ch){
				ret += dfs_count(next);
			}

			sub_tree_size[pos] = ret;
			return ret;
		};

		int root = 0;

		dfs_count(0);

		node_pos.resize(n);
		vector<bool> visit(n, false);
		queue<int> q;
		q.push(root);
		while(!q.empty()){
			int pos = q.front(); q.pop();
			decomposited_tree.push_back({pos});
			node_pos[pos] = {decomposited_tree.size()-1, 0};

			while(true){
				int max_size = 0;
				int next = -1;
				for(int to : Tree[pos].ch){
					if(max_size < sub_tree_size[to]){
						
						if(next>=0) q.push(next);

						max_size = sub_tree_size[to];
						next = to;
					}else{
						q.push(to);
					}
				}

				if(next < 0) break;

				decomposited_tree.back().push_back(next);
				node_pos[next] = {decomposited_tree.size()-1, decomposited_tree.back().size()-1};
				visit[next] = true;
				pos = next;
			}
		}
	}
};


int main(){
	int n,m;
	cin >> n >> m;
	vector<vector<int>> G(n);
	for(int i=0; i<m; i++){
		int u,v;
		cin >> u >> v;
		G[u].push_back(v);
		G[v].push_back(u);
	}

	HL_Decomposition HLD(G);

	for(auto& v: HLD.decomposited_tree){
		for(auto& x: v){
			cout << x << " " ;
		}
		cout << endl;
	}
	return 0;
}