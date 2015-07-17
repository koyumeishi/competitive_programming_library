#include <cmath>
#include <vector>

using namespace std;


class LCA{

	int root;
	//depth of node[i]
	vector<int> d;
	
	//parent of node[i]
	vector<vector<int> > p;

	
	int size;
	
	//initialization of depths and parents
	void init(vector<vector<int> > &g, int pos, int last, int v){
		d[pos] = v;
		p[0][pos] = last;
		for(int i=0; i<g[pos].size(); i++){
			if(g[pos][i] == last) continue;
			init(g, g[pos][i], pos, v+1);
		}
	}

public:

	//vector<vector<int> >
	//g[i][j] := node[i]'s edge gose to node[ g[i][j] ]
	
	LCA(vector<vector<int> > &g, int r){
		int n = g.size();
		root = r;
		d = vector<int>(n);

		size = 1;
		while((1<<size) <= n){
			size++;
		}
		
		p = vector<vector<int> >(size, vector<int>(n));
		
		init(g, root,-1,0);
		
		for(int k=0; k+1 < size; k++){
			for(int v=0; v<n; v++){
				if(p[k][v] < 0) p[k+1][v] = -1;
				else p[k+1][v] = p[k][p[k][v]];
			}
		}
	}
	
	LCA(vector<vector<int> > &g){
		int n = g.size();
		root = 0;
		d = vector<int>(n);
		size = 1;
		while((1<<size) <= n){
			size++;
		}
		p = vector<vector<int> >(size, vector<int>(n));

		//initialize as root = 0
		init(g, root,-1,0);
		
		for(int k=0; k+1 < size; k++){
			for(int v=0; v<n; v++){
				if(p[k][v] < 0) p[k+1][v] = -1;
				else p[k+1][v] = p[k][p[k][v]];
			}
		}
	}
	
	//return the node number of lowest common ancestor between u and v
	int get_lca(int u, int v){
		if(d[u] > d[v]) swap(u,v);
		for(int k=0; k<size; k++){
			if( (d[v] - d[u]) >> k & 1){
				v = p[k][v];
			}
		}
		if(u==v) return u;

		for(int k=size-1; k>=0; k--){
			if(p[k][u] != p[k][v]) {
				u = p[k][u];
				v = p[k][v];
			}
		}
		return p[0][u];
	}

	//return depth of node[u]
	int depth(int u){
		return d[u];
	}
};