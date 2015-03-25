#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <map>
#include <cstdlib>
#include <cmath>
#include <climits>
#include <functional>
using namespace std;


//segment tree
//using std::function as comparing function
template<class Value = int>
class SegmentTree{
	int n;
	vector<Value> V;
	Value DEFAULT_VALUE;

	//evaluation function
	Value default_evaluate(Value a, Value b){
		return max(a,b);
	}

	function< Value(Value, Value) > evaluate;

	//return evaluated value in [a,b)
	//T[at] covers [l,r)
	Value RangeEvaluation(int a, int b, int at, int l, int r){
		//out of range
		if(r <= a || b <= l) return DEFAULT_VALUE;
		//covered
		if(a <= l && r <= b) return V[at];

		//partially covered
		else{
			Value val_left = RangeEvaluation(a,b, at*2+1, l, (l+r)/2);
			Value val_right = RangeEvaluation(a,b, at*2+2, (l+r)/2, r);
			return evaluate(val_left, val_right);
		}
	}

public:
	SegmentTree(int size, Value DEFAULT = 0, function< Value(Value, Value) > eval = default_evaluate){
		DEFAULT_VALUE = DEFAULT;
		evaluate = eval;
		n=1;
		while(n<size) n <<= 1;
		V = vector<Value>(2*n - 1, DEFAULT_VALUE);
	}

	void update(int at, Value new_val){
		at += n-1;
		V[at] = new_val;
		while(at>0){
			at = (at-1)/2;
			V[at] = evaluate(V[at*2 + 1], V[at*2 + 2]);
		}
	}


	//return evaluated value in [l,r)
	Value RangeEvaluation(int l, int r){
		if(l>=r) return DEFAULT_VALUE;
		if(l>=n) return DEFAULT_VALUE;
		return RangeEvaluation(l,r, 0, 0, n);
	}
};





//segment tree
//using template for comparing
template<class Value = int>
struct comp_segmenttree{
	Value operator()(const Value &a, const Value &b) const{
		return a&b;
	}
};


template<class Value = int, class Comp = comp_segmenttree<Value> >
class SegmentTree{
	int n;
	vector<Value> V;
	Value DEFAULT_VALUE;

	Value evaluate(Value a, Value b){
		return Comp()(a,b);
	}

	//return evaluated value in [a,b)
	//T[at] covers [l,r)
	Value RangeEvaluation(int a, int b, int at, int l, int r){
		//out of range
		if(r <= a || b <= l) return DEFAULT_VALUE;
		//covered
		if(a <= l && r <= b) return V[at];

		//partially covered
		else{
			Value val_left = RangeEvaluation(a,b, at*2+1, l, (l+r)/2);
			Value val_right = RangeEvaluation(a,b, at*2+2, (l+r)/2, r);
			return evaluate(val_left, val_right);
		}
	}

public:
	SegmentTree(int size, Value DEFAULT = 0){
		DEFAULT_VALUE = DEFAULT;
		n=1;
		while(n<size) n <<= 1;
		V = vector<Value>(2*n - 1, DEFAULT_VALUE);
	}

	void update(int at, Value new_val){
		at += n-1;
		V[at] = new_val;
		while(at>0){
			at = (at-1)/2;
			V[at] = evaluate(V[at*2 + 1], V[at*2 + 2]);
		}
	}


	//return evaluated value in [l,r)
	Value RangeEvaluation(int l, int r){
		if(l>=r) return DEFAULT_VALUE;
		if(l>=n) return DEFAULT_VALUE;
		return RangeEvaluation(l,r, 0, 0, n);
	}
};
