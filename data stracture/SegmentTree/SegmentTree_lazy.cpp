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

	vector<T> value;
	vector<T> lazy_value;
	vector<bool> lazy;

	int size;

	void initialize(int size_){
		size = 1;
		while(size < size_) size <<= 1;

		value.resize(size<<1);
		lazy_value.resize(size<<1);
		lazy.resize(size<<1, false);
	}

	T evaluate(T x, T y){
		return x + y;
	}

	T lazy_evaluate(T x, T y){
		return x + y;
	}

	T translate_lazy_value(T x, int len){
		return x * len;
	}

	int get_cover_length(int pos){
		int len = 1;
		while(pos>0){
			pos = (pos-1)>>1;
			len <<= 1;
		}
		return len;
	}

	T translate_lazy_value(int pos){
		return lazy_value[pos] * get_cover_length(pos);
	}

	void push(int pos){
		if(lazy[pos] == false) return;

		if(pos < size-1){
			int l = (pos<<1)+1;
			int r = l+1;
			lazy_value[l] = lazy_evaluate(lazy_value[l], lazy_value[pos]);
			lazy_value[r] = lazy_evaluate(lazy_value[r], lazy_value[pos]);

			T new_val = evaluate( evaluate(value[l], translate_lazy_value(l)) ,
					evaluate(value[r], translate_lazy_value(r)) );
			value[pos] = new_val;

		}else{
			value[pos] = lazy_evaluate(value[pos], translate_lazy_value(pos) );
		}
		lazy_value[pos] = T();
		lazy[pos] = false;
	}

	void range_add(int left, int right, T add_value, int pos, int lb, int ub){
		push(pos);

		//out of range
		if(ub <= left || right <= lb) return;
		//covered
		if(left <= lb && ub <= right){
			lazy[pos] = true;
			lazy_value[pos] = lazy_evaluate(lazy_value[pos], add_value);
			return;
		}

		range_add(left, right, add_value, (pos<<1)+1, left, (left+right)>>1);
		range_add(left, right, add_value, (pos<<1)+2, (left+right)>>1, right);
	}

	//get value of [left,right)
	//value[pos] covers [lb, ub)
	T get_range_value(int left, int right, int pos, int lb, int ub){

		//out of range
		if(ub <= left || right <= lb) return T();
		//covered
		if(left <= lb && ub <= right){
			return evaluate(value[pos], translate_lazy_value(lazy_value[pos], ub-lb));
		}

		push(pos);

		T L = get_range_value(left, right, (pos<<1)+1, left, (left+right)>>1);
		T R = get_range_value(left, right, (pos<<1)+2, (left+right)>>1, right);

		return evaluate(L,R);
	}

	void lazy_update(int pos){
		if(pos>0) lazy_update( (pos-1)>>1 );
		push(pos);
	}

public:
	Segment_Tree_Lazy(int size_){
		initialize(size_);
	}

	Segment_Tree_Lazy(vector<T> vec){
		initialize(vec.size());

		for(int i=0; i<vec.size(); i++){
			update(i, vec[i]);
		}
	}

	void update(int pos, T new_value){
		pos += size - 1;
		lazy_update(pos);

		value[pos] = new_value;

		int len = 1;
		while(pos>0){
			pos = (pos-1)>>1;
			len <<= 1;

			value[pos] = evaluate(value[(pos<<1) + 1], value[(pos<<1) + 2]);
			translate_lazy_value(lazy_value[pos], len);
		}
	}

	//[l,r) += add_value
	void range_add(int left, int right, T add_value){
		range_add(left, right, add_value, 0, 0, size);
	}

	//get value of [left,right)
	T get_range_value(int left, int right){
		return get_range_value(left, right, 0, 0, size);
	}
};






