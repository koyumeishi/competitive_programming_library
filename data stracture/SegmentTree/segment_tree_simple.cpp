#include <vector>
using namespace std;

template<class T>
class segment_tree{
	vector<T> v;
	int len;

	int parent(int pos){return (pos-1)>>1;}
	int l_ch(int pos){return (pos<<1) + 1;}
	int r_ch(int pos){return (pos<<1) + 2;}

public:
	segment_tree(int size):len(1){
		while(len<=size) len<<=1;
		v.resize(len*2, T());
	}

	void set_value(int pos, int value){
		pos += len - 1;
		v[pos] = value;
		pos = parent(pos);
		while(1){
			v[pos] = v[l_ch(pos)] + v[r_ch(pos)];
			pos = parent(pos);
			if(pos==0) break;
		}
	}

	void set_range(int l,int r, T val, int lb,int ub,int pos){
		if(r<=lb || ub<=l) return;
		if(l<=lb && ub<=r){
			v[pos] = v[pos] + val;	// <- ********************
			return;
		}
		set_range(l,r,val, lb,(lb+ub)>>1, l_ch(pos));
		set_range(l,r,val, (lb+ub)>>1,ub, r_ch(pos));
		v[pos] = v[l_ch(pos)] + v[r_ch(pos)];	// <- ********************
	}

	//[l,r)
	void set_range(int l,int r, T val){set_range(l,r,val, 0,len, 0);}


	T get_range(int l,int r, int lb,int ub,int pos){
		if(r<=lb || ub<=l) return T();
		if(l<=lb && ub<=r){
			return v[pos];
		}
		T l_val = get_range(l,r,lb,(lb+ub)>>1, l_ch(pos));
		T r_val = get_range(l,r,(lb+ub)>>1,ub, r_ch(pos));
		return l_val+r_val;  // <- ********************
	}
	//[l,r)
	T get_range(int l, int r){return get_range(l,r, 0,len, 0);}

};