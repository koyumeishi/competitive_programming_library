#include <iostream>
#include <vector>

using namespace std;

template<class T = int>
class BinaryIndexedTree_1_indexed{
	void init(const vector<T> &A){
		for(int i=0; i<N; i++){
			add(i+1, A[i]);
		}
	}
	
public:
	vector<T> tree;
	int N;
	
	BinaryIndexedTree_1_indexed(const int n) : tree(n+1,0), N(n){
		
	}
	
	BinaryIndexedTree_1_indexed(const vector<T> &A) : tree(A.size()+1,0), N(A.size()){
		init(A);
	}

	//caution : position "i" must be 1-indexed
	void add(int i, const T x){
		while(i <= N){
			tree[i] += x;
			i += i & -i;
		}
	}

	//update node i to zero
	void to_zero(const int i){
		T val = get_at(i);
		add(i, -val);
	}

	//get sums [0,i]
	T get_sum(int i){
		T ret=0;
		while(i>0){
			ret += tree[i];
			i -= i & -i;
		}
		return ret;
	}

	//get sums [from,to]
	T get_sums_range(const int from, const int to){
		return get_sum(to) - get_sum(from-1);
	}

	//get at [i]
	T get_at(const int i){
		return get_sum(i) - get_sum(i-1);
	}

	int lower_bound(T val){
		if(val<=0) return 0;
		int x = 0;
		int k = 1;
		while((k<<1) <= N) k<<=1;
		
		for( ; k>0; k>>=1){
			if( x+k <= N && tree[x+k] < val ){
				val -= tree[x+k];
				x += k;
			}
		}
		return x+1;
	}

	void print(){
		for(int i=0; i<=N; i++){
			cerr << tree[i] << " ";
		}
		cerr << endl;
	}
};


//(lb,ub] )
template<class T = int>
int find_pos(BinaryIndexedTree_1_indexed<T> &BIT, int val){
	int lb = 0;
	int ub = BIT.N;
	while(ub-lb>1){
		int mid = (lb+ub)/2;
		T x = BIT.get_sum(mid);
		if(val <= x){
			ub = mid;
		}else{
			lb = mid;
		}
	}
	return ub;
}

#include <iostream>

//test
int main(){
	BinaryIndexedTree_1_indexed<long long> BIT(12);
	for(int i=1; i<=12; i++){
		BIT.add(i, 1);
	}

	cout << "input number of test cases" << endl;
	int n;
	cin >> n;

	for(int i=0; i<n; i++){
		BIT.print();

		//0 lowerbound(k)
		//1 add( k, x )

		cout << "input query type" << endl;
		cout << "0 k : lower_bound(k)" << endl;
		cout << "1 k x : add x at k" << endl;
		
		int t;
		cin >> t;
		
		if(t==0){
			int k;
			cin >> k;
			cout << BIT.lower_bound(k) << endl;
		}else{
			int k;
			long long x;
			cin >> k >> x;
			BIT.add(k,x);
		}
		
	}
}