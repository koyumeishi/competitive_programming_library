#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;
class XorShift{
public:
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t w;

	XorShift(){
		x = 123456789;
		y = 362436069;
		z = 521288629;
		w = 88675123;
	}
	
	XorShift(uint32_t seed){
		x = 123456789;
		y = 362436069;
		z = 521288629;
		w = seed;
	}
	
	uint32_t xor128(void) {
		uint32_t t = x ^ (x << 11);
		
		x = y; y = z; z = w;
		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
	}
};


template<class Type=int>
class RBST{
public:
	
	static const Type MIN_VAL = -100000000;
	static const Type MAX_VAL = +100000000;
	
	class Node{
	public:
		Type value;
		Node* left;
		Node* right;

		int sub_tree_size;
		
		//sub tree + pos
		Type sub_tree_sum;
		Type sub_tree_min;
		Type sub_tree_max;

		//reversed flag
		bool is_reversed;

		//range added flag
		Type range_added_val;
		
		Node(Type val) :
		value(val),
		
		left((Node*)NULL),
		right((Node*)NULL),
		
		sub_tree_size(1),
		sub_tree_sum(val),
		sub_tree_min(val),
		sub_tree_max(val),

		is_reversed(false),
		range_added_val(0){
			
		}
	};

	Node* root;

	XorShift x_rand;
	
	unsigned int get_rand(){
		return x_rand.xor128();
	}

	RBST(){
		root = (Node*)NULL;
		x_rand = XorShift((unsigned)time(NULL));
	}

	int size(Node* t){
		if(t != NULL) return t->sub_tree_size;
		return 0;
	}
	
	Type sum(Node* t){
		if(t!=NULL){
			Type ret = t->sub_tree_sum;
			ret += t->range_added_val * t->sub_tree_size;
			return ret;
		}
		else return 0;
	}
	Type get_min(Node* t){
		if(t!=NULL){
			Type ret = t->sub_tree_min;
			ret += t->range_added_val;
			return ret;
		}
		else return MAX_VAL;
	}
	
	Type get_max(Node* t){
		if(t!=NULL){
			Type ret = t->sub_tree_max;
			ret += t->range_added_val;
			return ret;
		}
		else return MIN_VAL;
	}
	
	void push_reverse(Node* t){
		
		if(t == NULL) return;
		
		//node is reversed
		if(t->is_reversed == true){
			swap(t->left, t->right);
			if(t->left) t->left->is_reversed ^= true;
			if(t->right) t->right->is_reversed ^= true;
			t->is_reversed = false;
		}
	}
	
	void push_range_add(Node* t){
		if(t == NULL) return;
		
		//added
		t->value += t->range_added_val;
		t->sub_tree_sum += t->range_added_val * t->sub_tree_size;
		t->sub_tree_max += t->range_added_val;
		t->sub_tree_min += t->range_added_val;
		
		if(t->left){
			t->left->range_added_val += t->range_added_val;
		}
		if(t->right){
			t->right->range_added_val += t->range_added_val;
		}
		
		t->range_added_val = 0;
	}

	void push(Node* t){
		//push_reverse(t);
		push_range_add(t);
	}


	void reverse(Node* t){
		if(t == NULL) return;
		t->is_reversed ^= true;
	}
	
	void add(Node* t, Type v){
		if(t == NULL) return;
		push(t);
		t->range_added_val += v;
		update(t);
	}
	
	Node* update(Node* t){
		t->sub_tree_size = size( t->left ) + size( t->right ) + 1;

		push(t);
		t->sub_tree_sum = sum( t->left ) + sum( t->right ) + t->value;
		
		t->sub_tree_max = max( max( get_max(t->left), get_max(t->right) ), t->value );
		
		t->sub_tree_min = min( min( get_min(t->left), get_min(t->right) ), t->value );
		
		return t;
	}

	Node* merge(Node* l, Node* r){
		if(l==NULL) return r;
		if(r==NULL) return l;

		push(r);
		push(l);
		
		int priority_left = get_rand();
		int priority_right = get_rand();

		if( priority_left > priority_right){
			l->right = merge(l->right, r);
			return update(l);
		}else{
			r->left = merge(l, r->left);
			return update(r);
		}
	}

	pair<Node*, Node*> split(Node* t, int k){
		if(t==NULL) return make_pair( (Node*)NULL, (Node*)NULL );

		push(t);
		
		if(k <= size( t->left ) ){
			pair<Node*, Node*> s = split(t->left, k);
			t->left = s.second;
			return make_pair( s.first, update(t) );
		}else{
			pair<Node*, Node*> s = split(t->right, k-size(t->left)-1);
			t->right = s.first;
			return make_pair( update(t), s.second );
		}
	}

	Node* insert(Node* t, int k, Type val){
		pair<Node*, Node*> s = split(t, k);
		t = merge(s.first, new Node(val));
		t = merge(t, s.second);

		return update(t);
	}

	Node* erase(Node* t, int k){
		pair<Node*, Node*> s1 = split(t, k+1);
		pair<Node*, Node*> s2 = split(s1.first, k);

		t = merge(s2.first, s1.second);
		return update(t);
	}

	Node* find(Node* t, int k){
		int c = size(t->left);
		if(k<c) return find(t->left, k);
		if(k>c) return find(t->right, k-c-1);
		return t;
	}


	void insert(int k, Type val){
		root = insert(root, k, val);
	}

	void erase(int k){
		root = erase(root, k);
	}

	Node* find(int k){
		return find(root, k);
	}


	
	//split at pos[i]
	//return root nodes begin with [ 0, pos[0], pos[1], ... , pos[n-1] ]
	vector<Node*> split_at(Node* t, const vector<int> &pos){
		vector<Node*> ret(pos.size()+1);
		for(int i=pos.size()-1; i>=0; i--){
			pair<Node*, Node*> p = split(t, pos[i]);
			ret[i+1] = p.second;
			t = p.first;
		}
		ret[0] = t;
		
		return ret;
	}
	
	//merge nodes in vector<Node*> v
	Node* merge_vec(const vector<Node*> &v){
		Node* r = v[0];
		for(int i=1; i<v.size(); i++){
			r = merge(r, v[i]);
		}
		return r;
	}

	//[l,r) <-> [x,y)
	void swap_range(int l, int r, int x, int y){
		//((((( [0,l) + [l,r) + [r,x) + [x,y) + [y,n) ]]]]]
		vector<Node*> v = split_at(root, {l,r,x,y});
		swap(v[1], v[3]);
		root = merge_vec(v);
	}

	//reverse range[l,r] -> [r,l]
	void reverse_range(int l, int r){
		vector<Node*> v = split_at(root, {l,r+1});
		reverse(v[1]);
		root = merge_vec(v);
	}

	//add range [l,r].val += val
	void add_range(int l, int r, int val){
		vector<Node*> v = split_at(root, {l,r+1});
		add(v[1], val);
		root = merge_vec(v);
	}

	//get minimum in range [l,r]
	Type get_range_min(int l, int r){
		vector<Node*> v = split_at(root, {l,r+1});
		push(v[1]);
		Type ret = v[1]->sub_tree_min;
		root = merge_vec(v);
		return ret;
	}

	//get maxmum in range [l,r]
	Type get_range_max(int l, int r){
		vector<Node*> v = split_at(root, {l,r+1});
		push(v[1]);
		Type ret = v[1]->sub_tree_max;
		root = merge_vec(v);
		return ret;
	}

	void get_all_value(Node* t, int offset, vector<Type> &v){
		if(t==NULL) return;
		
		push(t);

		int c = size(t->left);
		v[ c + offset ] = t->value;
		get_all_value( t->left, offset, v);
		get_all_value( t->right, c+1+offset, v);
	}

	void get_all_value(vector<Type> &v){
		get_all_value(root, 0, v);
	}

	//get [l,r)
	void get_range_value(Node* t, int offset, vector<Type> &v, const int &l, const int &r){
		if(t==NULL) return;
		
		push(t);

		int c = size(t->left);
		int pos = c+offset;
		if( l<= pos && pos < r ) v[ pos-l ] = t->value;
		if(offset < r && l <= pos-1) get_range_value( t->left, offset, v, l, r);
		if(pos+1 < r && l <= pos+size(t->right)) get_range_value( t->right, pos+1, v, l, r);
	}

	//get [l,r)
	void get_range_value(vector<Type> &v, const int &l, const int &r){
		get_range_value(root, 0, v, l, r);
	}

};


//test
#include <iostream>
int main(){
	int n;
	cin >> n;
	
	RBST t;
	
	for(int i=0; i<n; i++){
		int x;
		cin >> x;
		t.insert(i,x);
	}

	for(int i=0; i<n; i++){
		cout << t.find(i)->value << " ";
	}
	cout << endl << endl;
	
	//query
	// 1 l r x y := swap [l,r] [x,y]
	// 2 l r := reverse [l,r]
	int m;
	cin >> m;
	for(int i=0; i<m; i++){
		int type;
		cin >> type;
		if(type==1){
			int l,r,x,y;
			cin >> l >> r >> x >> y;
			t.swap_range(l, r+1, x, y+1);
			
		}else if(type==2){
			int l,r;
			cin >> l >> r;
			t.reverse_range(l,r);
		}else if(type==3){
			int l,r,v;
			cin >> l >> r >> v;
			t.add_range(l,r,v);
		}else if(type==4){
			int l,r;
			cin >> l >> r;
			cout << "min " << t.get_range_min(l,r) << endl;
			cout << "max " << t.get_range_max(l,r) << endl;
		}else{
			//(get value[l,r)]
			int l,r;
			cin >> l >> r;
			vector<int> v(r-l);
			t.get_range_value(v, l,r);
			for(int i=0; i<v.size(); i++){
				cout << v[i] << " ";
			}
			cout << endl;
			
		}
		vector<int> v(n);
		t.get_all_value(t.root, 0, v);
		for(int i=0; i<n; i++){
			cout << v[i] << " ";
		}
		cout << endl;
		
	}
	
}