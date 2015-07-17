
/*

xorの問題でバイナリ行列を使うことで O(N^3) -> O(N * M) になる。（Mはビット長）

Ax=bの解xを求める
gaussian_elimination_binary
拡大係数行列を作るので p+m が 64bit をはみ出す場合は bitset を使うこと

行列Aの係数行列のランクを求める
get_rank

rank(A) = rank(TA) なので適宜変形すると良い


入力は vector<bit> 
32bit で足りる ->int
64bit -> long long
それ以上 -> bitset<length>

*/

#include <vector>
#include <bitset>
using namespace std;

// A[n*p] * X[p*m] = B[n*m]
template<class T = long long>
vector<T> gaussian_elimination_binary(vector<T> A, vector<T> B, int n, int p, int m){
	vector<T> M(n);
	for(int i=0; i<n; i++){
		M[i] = A[i] << m;
		M[i] |= B[i];
	}

	int rank = 0;
	vector<int> left_most(n,-1);
	for(int row = 0, col = p+m-1; row<n && col>=m; col--){
		if( ((M[row] >> col) & T(1)) == T(0)){
			int pivot = row;
			for(int row__ = row; row__<n; row__++){
				if( ((M[row__] >> col) & T(1)) != T(0) ){
					pivot = row__;
					break;
				}
			}

			if(pivot == row) continue;
			swap(M[row], M[pivot]);
		}

		rank++;
		left_most[row] = col;

		for(int row__=row+1; row__<n; row__++){
			if( ((M[row__] >> col) & T(1)) != T(0) ) M[row__] ^= M[row];
		}

		row++;
	}

	// T = long long 
	//T mask_B = ((T)1<<m) - 1;
	//T mask_A = (((T)1<<(m+p)) - 1) ^ mask_B;

	// T = bitset
	T mask_B( string(m, '1') );
	T mask_A( string(m+p, '1') );
	mask_A ^= mask_B;

	bool has_solution = true;
	for(int row=n-1; row>=0; row--){
		if( (M[row] & mask_B) != T(0) && (M[row] & mask_A) == T(0) ) has_solution = false;
		if(left_most[row] == -1) continue;
		int col = left_most[row];
		for(int row__ = row-1; row__>=0; row__--){
			if( ((M[row__] >> col) & T(1)) != T(0) ) M[row__] ^= M[row];
		}
	}

	return has_solution? M : vector<T>(0);
}

template<class T = long long>
int get_rank(vector<T> A, int n, int m){
	int rank = 0;
	for(int row=0, col=0; col<m && row<n; col++){
		if( ((A[row] >> col)&T(1)) == T(0) ){
			int pivot = row;
			for(int row__ = row; row__<n; row__++){
				if( ((A[row__] >> col) & T(1)) != T(0) ){
					pivot = row__;
					break;
				}
			}

			if(pivot == row) continue;
			swap(A[row], A[pivot]);
		}

		rank++;

		for(int row__=row+1; row__<n; row__++){
			if( ((A[row__] >> col) & T(1)) != T(0) ) A[row__] ^= A[row];
		}
		row++;
	}
	return rank;
}


#include <iostream>

int main(){

	int n;
	cin >> n;
	vector<int> d(n), w(n);
	for(int i=0; i<n; i++) cin >> d[i];
	for(int i=0; i<n; i++) cin >> w[i];

	vector< bitset<200> > A(n);
	for(int i=0; i<n; i++){
		d[i] %= n;
		int right = (i + d[i]) % n;
		int left = (i - d[i] + n) % n;
		A[right].set(i, 1);
		A[left].set(i, 1);
	}

	vector< bitset<200> > B(n);
	for(int i=0; i<n; i++){
		B[i].set(0, 1^w[i]);
	}

	auto v = gaussian_elimination_binary< bitset<200> >(A,B, n,n,1);

	cout << ((v.size() == 0)?"No":"Yes") << endl;

	return 0;
}