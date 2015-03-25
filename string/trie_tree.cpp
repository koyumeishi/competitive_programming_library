
class Trie_Tree{
public:
	class Node{
	public:
		int depth;
		Node* edge[26];
		Node(int d){
			for(int i=0; i<26; ++i){
				edge[i] = NULL;
			}
			depth = d;
		}

	};
	
	Node* add(Node* t, const string &s, int pos){
		if(pos==s.size()) return t;
		
		if(t->edge[ s[pos] - 'a' ] == NULL){
			t->edge[s[pos] - 'a'] = new Node(pos+1);
			pool.push_back(t->edge[s[pos] - 'a']);
			
		}
		t->edge[s[pos] - 'a'] = add(t->edge[s[pos] - 'a'], s, pos+1);
		
		return t;
	}

	int get_depth(Node* t, const string &s, const int &pos){
		if(t==NULL) return pos;
		if(s.size() == pos) return t->depth;
		return get_depth( t->edge[ s[pos] - 'a' ], s, pos+1 );
	}
	
	vector<Node*> pool;
	Node* root;
	
	Trie_Tree(){
		root = new Node(0);
		pool.push_back(root);
	}

	~Trie_Tree(){
		for(int i=0; i<pool.size(); i++){
			delete(pool[i]);
		}
	}

	int get_d(string &s){
		return get_depth(root, s, 0);
	}

	int insert(string &s){
		root = add(root, s, 0);
	}

};
