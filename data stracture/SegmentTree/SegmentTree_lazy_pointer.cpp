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

	struct node{
		T value = T();
		T lazy_value = T();
		bool lazy = false;
		int lb = 0;
		int ub = 0;
		node* par = NULL;
		node* lch = NULL;
		node* rch = NULL;
	};

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

		v.resize(size<<1);
		root = constract(&v[0], 0, 0, size);
	}

	T get_value(node* t){
		if(t==NULL) return T();
		T ret = t->value;

		if(t->lazy){
			ret += t->lazy_value * (t->ub - t->lb);
		}

		return ret;
	}

	void push(node* t){
		if(t==NULL) return;
		if(t->lazy){
			if(t->lch != NULL){
				t->lch->lazy_value += t->lazy_value;
				t->lch->lazy = true;

				t->rch->lazy_value += t->lazy_value;
				t->rch->lazy = true;

				t->value = get_value(t->lch) + get_value(t->rch);
			}else{
				t->value = get_value(t);
			}

			t->lazy_value = T();
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
			t->lazy_value += add_value;
			t->lazy = true;
			return;
		}

		range_add(left, right, add_value, t->lch);
		range_add(left, right, add_value, t->rch);

		t->value = get_value(t->lch) + get_value(t->rch);

	}

	//get value of [left,right)
	T get_range_value(int left, int right, node* t){
		if(t==NULL) return T();
		if(t->ub <= left || right <= t->lb){
			return T();
		}

		push(t);
		if(left <= t->lb && t->ub <= right){
			return get_value(t);
		}

		T L = get_range_value(left, right, t->lch);
		T R = get_range_value(left, right, t->rch);

		return L+R;
	}

	void lazy_update(node* t){
		if(t->par != root){
			lazy_update(t->par);
		}
		push(t);
	}

public:
	Segment_Tree_Lazy(int size_){
		initialize(size_);
	}

	void update(int pos, T new_value){
		node* t = &v[pos + size-1];
		
		lazy_update(t);

		t->value = new_value;

		while(t != root){
			t = t->par;
			t->value = get_value(t->lch) + get_value(t->rch);
		}
	}

	//[l,r) += add_value
	void range_add(int left, int right, T add_value){
		range_add(left, right, add_value, root);
	}

	//get value of [left,right)
	T get_range_value(int left, int right){
		return get_range_value(left, right, root);
	}
};


template<class T=int>
class Segment_Tree_Lazy{
	static const T default_value = 1<<29;
	static const T default_lazy_value = 0;

	struct node{
		T value;
		T lazy_value;
		bool lazy;
		int lb;
		int ub;
		node* par;
		node* lch;
		node* rch;

		node(){
			value = default_value;
			lazy_value = default_lazy_value;
			lazy = false;
			lb = 0;
			ub = 0;
			par = NULL;
			lch = NULL;
			rch = NULL;
		}

	};

	T evaluate(T x, T y){
		return min(x,y);
	}

	T evaluate_T_L(T x, T y){
		return x+y;
	}


	void new_lazy(node* t, T add_value){
		if(t==NULL) return;
		t->lazy_value = add_value;
		t->lazy = true;
	}

	T lazy_to_value(node* t){
		if(t->lazy) return t->lazy_value;
		return default_lazy_value;
	}

	T get_value(node* t){
		if(t==NULL) return default_value;
		return evaluate_T_L(t->value , lazy_to_value(t));
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

		v.resize(size<<1);
		root = constract(&v[0], 0, 0, size);
	}

	void push(node* t){
		if(t==NULL) return;
		if(t->lazy){
			if(t->lch != NULL){
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
	Segment_Tree_Lazy(int size_){
		initialize(size_);
	}

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

	//get value of [left,right)
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

	Segment_Tree_Lazy_<int> seg(n);
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