#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <map>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <stack>

using namespace std;

//V := type of node value
//U := type of lazy value
template<class V=int, class U=int>
class Dynamic_SegmentTree{
	struct node{	//[l,r)
		node* left;
		node* right;
		V val;
		U lazy;
		bool is_lazy;
		node(V v, U u) : 
			left(nullptr), right(nullptr),
			val(v), lazy(u),
			is_lazy(false)
		{}
	};

	vector<node*> pool;
	node* root;
	V default_v;
	U default_l;
	long long size;

	//range add
	V evaluate_vv(V a, V b){
		return a+b;
	}
	U evaluate_ll(U a, U b){
		return a+b;
		//return b; //range fill
	}
	V evaluate_vl(node* t, long long lb, long long ub){
		if(t->is_lazy) return t->val + t->lazy * (ub - lb);
		return t->val;
	}

	void update(node* t, long long lb, long long ub){
		if(ub-lb == 1){
			//t->val = evaluate_vl(t, lb, ub);
			return;
		}
		V l_val = (t->left != nullptr) ? evaluate_vl(t->left, lb, (lb+ub)/2) : default_v;
		V r_val = (t->right != nullptr) ? evaluate_vl(t->right, (lb+ub)/2, ub) : default_v;

		t->val = evaluate_vv(l_val, r_val);
	}


	inline void set_children(node* t, bool l, bool r){
		if(l && t->left == nullptr){
			t->left = new node(default_v, default_l);
			pool.push_back(t->left);
		}

		if(r && t->right == nullptr){
			t->right = new node(default_v, default_l);
			pool.push_back(t->right);
		}
	}

	void propagete(node* t, long long lb, long long ub){
		if(t->is_lazy == false) return;
		if(ub - lb == 1){
			//t->val = evaluate_vl(t, lb, ub);
			return;
		}else{
			set_children(t, true, true);
			for(node* ch : {t->left, t->right}){
				if(ch->is_lazy){
					ch->lazy = evaluate_ll(ch->lazy, t->lazy);
				}else{
					ch->lazy = t->lazy;
				}
				ch->is_lazy = true;

				//ch->val = default_v;	//range fill
			}
		}

		t->lazy = default_l;
		t->is_lazy = false;
		update(t, lb, ub);
	}

	void range_update(node* t, long long t_lb, long long t_ub, long long lb, long long ub, U new_val){
		if(ub <= t_lb || t_ub <= lb) return;	//out of range
		propagete(t, t_lb, t_ub);

		if(lb <= t_lb && t_ub <= ub){	// fully covered
			if(t->is_lazy) t->lazy = evaluate_ll(t->lazy, new_val);
			else t->lazy = new_val;
			t->is_lazy = true;

			propagete(t, t_lb, t_ub);

			return;
		}

		//partially covered
		bool l = !(ub <= t_lb || (t_lb+t_ub)/2 <= lb);
		bool r = !(ub <= (t_lb+t_ub)/2 || t_ub <= lb);
		set_children(t, l,r);

		if(l) range_update(t->left, t_lb, (t_lb+t_ub)/2, lb,ub, new_val);
		if(r) range_update(t->right, (t_lb+t_ub)/2, t_ub, lb,ub, new_val);

		update(t, t_lb, t_ub);
	}

	V range_evaluate(node* t, long long t_lb, long long t_ub, long long lb, long long ub){
		if(ub <= t_lb || t_ub <= lb) return default_v;	//out of range
		propagete(t, t_lb, t_ub);

		if(lb <= t_lb && t_ub <= ub){	// fully covered
			return evaluate_vl(t, t_lb, t_ub);
		}

		//partially covered
		bool l = !(ub <= t_lb || (t_lb+t_ub)/2 <= lb);
		bool r = !(ub <= (t_lb+t_ub)/2 || t_ub <= lb);
		set_children(t, l,r);

		V l_val = l ? range_evaluate(t->left, t_lb, (t_lb+t_ub)/2, lb,ub) : default_v;
		V r_val = r ? range_evaluate(t->right, (t_lb+t_ub)/2, t_ub, lb,ub) : default_v;

		return evaluate_vv(l_val, r_val);
	}

public:
	Dynamic_SegmentTree(long long size_, V default_value=0, U default_lazy=0) :
		default_v(default_value), default_l(default_lazy)
	{
		long long sz = 1;
		while(sz < size_) sz <<= 1LL;

		size = sz;

		root = new node(default_value, default_lazy);
		pool.push_back(root);
	}

	~Dynamic_SegmentTree(){
		for(int i=0; i<pool.size(); i++) delete(pool[i]);
	}

	// A[pos] <- set_val
	void set(long long pos, V set_val){
		node* t = root;
		long long t_lb = 0;
		long long t_ub = size;
		stack<node*> s;
		stack<long long> lb,ub;
		s.push(t);
		lb.push(t_lb);	ub.push(t_ub);

		while(t_ub - t_lb != 1){
			bool l = pos < (t_lb + t_ub) / 2;
			set_children(t, l, !l);
			propagete(t, t_lb, t_ub);

			if( l ){
				t = t->left;
				t_ub = (t_lb+t_ub)/2;
			}else{
				t = t->right;
				t_lb = (t_lb+t_ub)/2;
			}
			s.push(t);
			lb.push(t_lb);	ub.push(t_ub);
		}

		propagete(t, t_lb, t_ub);
		t -> val = set_val;
		s.pop(); lb.pop(); ub.pop();

		while(s.size()){
			t = s.top();
			t_lb = lb.top(); t_ub = ub.top();
			s.pop(); lb.pop(); ub.pop();
			update(t, t_lb, t_ub);
		}
	}

	void range_update(long long lb, long long ub, U new_val){
		range_update(root, 0, size, lb, ub, new_val);
	}

	V range_evaluate(long long lb, long long ub){
		return range_evaluate(root, 0, size, lb, ub);
	}

	void dbg(){
		for(auto t: pool){
			cerr << t->val << " " << t->lazy << endl;
		}
	}
};


//test
int main(){
	cout << "input array length" << endl;
	int n;
	cin >> n;
	cout << "input array value" << endl;
	vector<int> a(n);
	for(int i=0; i<n; i++){
		cin >> a[i];
	}

	Dynamic_SegmentTree<int,int> seg(n);
	for(int i=0; i<n; i++){
		seg.set(i, a[i]);
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
			seg.set(x,y);

		}else if(t==1){
			int x,y,z;
			cin >> x >> y >> z;
			seg.range_update(x,y,z);

		}else if(t==2){
			int x,y;
			cin >> x >> y;
			cout << (int)seg.range_evaluate(x,y) << endl;

		}

		seg.dbg();
	}
	return 0;
}