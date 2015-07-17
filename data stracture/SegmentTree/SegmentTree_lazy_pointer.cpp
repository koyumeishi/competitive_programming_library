#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <map>
#include <cstdlib>
#include <cmath>
#include <climits>

using namespace std;

template<class T=int>
class Segment_Tree_Lazy{
private:
	//default values are set in the constractor
	const T default_value;		//default (NULL) node value
	const T default_lazy_value;	//default lazy value

	struct node{
		T value; T lazy_value;
		bool lazy; int lb; int ub;
		node* par; node* lch; node* rch;

		node(T default_value, T default_lazy_value) : 
			value(default_value),
			lazy_value(default_lazy_value),
			lazy(false),
			lb(0),ub(0),			//this node covers [lb, rb)
			par(NULL),lch(NULL),rch(NULL){
		}
	};

	T evaluate(T left_val, T right_val){	//evaluate node value
		return left_val + right_val;	//sum
		//return max(left_val, right_val);	//max
	}

	T evaluate_node_and_lazy(T node_val, T lazy_val){	//evaluate node value and lazy value
		return node_val + lazy_val;	//sum
		//return max(node_val, lazy_val);	//max
		//return lazy_val;	//fill
	}
	T evaluate_lazy_and_lazy(T old_val, T new_val){	
		return old_val + new_val;	//sum
		//return new_val; 	//fill
	}

	void new_lazy(node* t, T add_value){	
		if(t==NULL) return;
		if(t->lazy){
			t->lazy_value = evaluate_lazy_and_lazy(t->lazy_value, add_value);
		}else{
			t->lazy_value = add_value;
		}
		t->lazy = true;
	}

	T lazy_to_value(node* t){
		if(t->lazy) return t->lazy_value * (t->ub - t->lb);
		return default_lazy_value;
	}

	T get_value(node* t){
		if(t==NULL) return default_value;
		return evaluate_node_and_lazy(t->value , lazy_to_value(t));
	}

	vector<node> v;
	node* root;

	int size;

	node* constract(node* t, int pos, int lb, int ub){
		if(pos == 0){
			t->par = t;
		}
		t->lb = lb;
		t->ub = ub;

		if(pos<size-1){
			t->lch = constract(&v[(pos<<1) + 1], (pos<<1) + 1, lb, (ub+lb)>>1);
			t->lch->par = t;

			t->rch = constract(&v[(pos<<1) + 2], (pos<<1) + 2, (ub+lb)>>1, ub);
			t->rch->par = t;
		}

		return t;
	}

	void initialize(int size_){
		size = 1;
		while(size < size_) size <<= 1;

		v = vector<node>(size<<1, node(default_value, default_lazy_value));
		root = constract(&v[0], 0, 0, size);
	}

	//propagate lazy value of node t to its children
	void push(node* t){
		if(t==NULL) return;
		if(t->lazy){
			if(t->lch != NULL){	//has child
				new_lazy(t->lch, t->lazy_value);
				new_lazy(t->rch, t->lazy_value);

				t->value = evaluate( get_value(t->lch), get_value(t->rch) );
			}else{
				t->value = get_value(t);
			}

			t->lazy_value = default_lazy_value;
			t->lazy = false;
		}
	}


	void range_add(int left, int right, T add_value, node* t){
		if(t==NULL) return;
		if(t->ub <= left || right <= t->lb){
			return;
		}

		push(t);
		if(left <= t->lb && t->ub <= right){
			new_lazy(t, add_value);
			return;
		}

		//partialy covered
		range_add(left, right, add_value, t->lch);
		range_add(left, right, add_value, t->rch);
		
		t->value = evaluate( get_value(t->lch), get_value(t->rch) );
	}

	//get value of [left,right)
	T get_range_value(int left, int right, node* t){
		if(t==NULL) return default_value;
		if(t->ub <= left || right <= t->lb){
			return default_value;
		}

		push(t);
		if(left <= t->lb && t->ub <= right){
			return get_value(t);
		}

		T L = get_range_value(left, right, t->lch);
		T R = get_range_value(left, right, t->rch);

		return evaluate(L, R);
	}

	void lazy_update(node* t){
		if(t->par != root){
			lazy_update(t->par);
		}
		push(t);
	}

public:
	//default node value must be set
	// sum : 0
	// max : MIN_INF
	// min : MAX_INF
	// gcd : 1
	Segment_Tree_Lazy(int size_, T default_value__ = 0, T default_lazy_value__ = 0) : 
		default_value(default_value__), default_lazy_value(default_lazy_value__){
		initialize(size_);
	}


	//node[pos] <= new_value
	void update(int pos, T new_value){
		node* t = &v[pos + size-1];
		
		lazy_update(t);

		t->value = new_value;

		while(t != root){
			t = t->par;
			t->value = evaluate( get_value(t->lch), get_value(t->rch) );
		}
	}

	//[l,r) += add_value
	void range_add(int left, int right, T add_value){
		range_add(left, right, add_value, root);
	}

	//get value [left,right)
	T get_range_value(int left, int right){
		return get_range_value(left, right, root);
	}

	void dbg(){
		for(int i=0; i<v.size(); i++){
			cerr << get_value(&v[i]) << " ";
		}
		cerr << endl;
	}
};


//test
int main(){
	int n;
	cin >> n;
	vector<int> a(n);
	for(int i=0; i<n; i++){
		cin >> a[i];
	}

	Segment_Tree_Lazy<int> seg(n);
	for(int i=0; i<n; i++){
		seg.update(i, a[i]);
	}

	cout << "input number of query\n";

	int q;
	cin >> q;
	while(q--){
		cout << "input query\n\t0 x y : update a[x] = y\n\t1 x y z : add z in the range [x,y)\n\t2 x y : get sums of the range [x,y)" << endl;
		int t;
		cin >> t;
		if(t==0){
			int x,y;
			cin >> x >> y;
			seg.update(x,y);

		}else if(t==1){
			int x,y,z;
			cin >> x >> y >> z;
			seg.range_add(x,y,z);

		}else if(t==2){
			int x,y;
			cin >> x >> y;
			cout << (int)seg.get_range_value(x,y) << endl;

		}

		seg.dbg();
	}
	return 0;
}