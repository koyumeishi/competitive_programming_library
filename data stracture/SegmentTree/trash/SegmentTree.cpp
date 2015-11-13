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

class SegmentTreeMIN{
	int n;
	vector<int> T;
	int INF;
public:
	SegmentTreeMIN(int size){
		INF = INT_MAX;
		n=1;
		while(n<size) n <<= 1;
		T.resize(2*n - 1);
		fill(T.begin(), T.end(), INF);
	}

	//basic array[k] <- val
	void update(int k, int val){
		//array[0] <=> T[n-1]
		k += n-1;
		T[k] = val;
		while(k>0){
			//k = (k-1)/2 == 0, then break
			k = (k-1)/2;
			T[k] = min(T[k*2 + 1], T[k*2 + 2]);
		}
	}
	//return minimum value [a,b)
	//T[k] covers [l,r)
	int RMQ(int a, int b, int k, int l, int r){
		//out of range
		if(r <= a || b <= l) return INF;
		//covered
		if(a <= l && r <= b) return T[k];
		//partially covered
		else{
			int val_left = RMQ(a,b, k*2+1, l, (l+r)/2);
			int val_right = RMQ(a,b, k*2+2, (l+r)/2, r);
			return min(val_left, val_right);
		}
	}
	int size(){
		return n;
	}
};


class SegmentTreeMAX{
	int n;
	vector<int> T;
public:
	SegmentTreeMAX(int size){
		n=1;
		while(n<size) n <<= 1;
		T.resize(2*n - 1);
		fill(T.begin(), T.end(), 0);
	}

	//basic array[k] <- val
	void update(int k, int val){
		//array[0] <=> T[n-1]
		k += n-1;
		T[k] = val;
		while(k>0){
			//k = (k-1)/2 == 0, then break
			k = (k-1)/2;
			T[k] = max(T[k*2 + 1], T[k*2 + 2]);
		}
	}
	//return minimum value [a,b)
	//T[k] covers [l,r)
	int RMQ(int a, int b, int k, int l, int r){
		//out of range
		if(r <= a || b <= l) return 0;
		//covered
		if(a <= l && r <= b) return T[k];
		//partially covered
		else{
			int val_left = RMQ(a,b, k*2+1, l, (l+r)/2);
			int val_right = RMQ(a,b, k*2+2, (l+r)/2, r);
			return max(val_left, val_right);
		}
	}
	int size(){
		return n;
	}
};

