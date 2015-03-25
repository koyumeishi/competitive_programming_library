#include <vector>

using namespace std;

class UnionFindTree{
	typedef struct {
		int parent;
		int rank;
	}base_node;
	
	vector<base_node> node;
public:
	UnionFindTree(int n){
		node.resize(n);
		for(int i=0; i<n; i++){
			node[i].parent=i;
			node[i].rank=0;
		}
	}

	int find(int x){
		if(node[x].parent == x) return x;
		else{
			return node[x].parent = find(node[x].parent);
		}
	}
	
	bool same(int x, int y){
		return find(x) == find(y);
	}

	void unite(int x, int y){
		x = find(node[x].parent);
		y = find(node[y].parent);
		if(x==y) return;
		if(node[x].rank < node[y].rank){
			node[x].parent = y;
		}else if(node[x].rank > node[y].rank){
			node[y].parent = x;
		}else{
			node[x].rank++;
			unite(x,y);
		}
	}
};
