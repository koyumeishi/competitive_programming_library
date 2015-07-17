
long long extgcd(long long a, long long b, long long &x, long long &y){
	long long d=a;
	if(b!=0){
		d = extgcd(b, a%b, y, x);
		y -= (a/b) * x;
	}else{
		x = 1;
		y = 0;
	}
	return d;
}

long long mod_inverse(long long a, long long m){
	long long x,y;
	extgcd(a,m,x,y);
	return (m+x%m)%m;
}

// mod 2 かつ rank についてのみは verified
// mod 2 は　xor の問題で良く使いそうなので、 逆元を求めずにbit演算だけでやるタイプも作っておいた方が良い（速度的に）
// http://yukicoder.me/problems/481 (2015/04/17)
// http://yukicoder.me/problems/127 (2015/04/17)

// A[n*p] * X[p*m] = B[n*m]
template<class T = int>
int gaussian_elimination_with_mod(vector<vector<T>>& A, vector<vector<T>>& B, int n, int p, int m, const T mod){
	int R = max(n,p); int C = p+m;
	vector<vector<T>> V( R, vector<T>(C, 0) );
	for(int i=0; i<n; i++)
		for(int j=0; j<p; j++) V[i][j] = A[i][j];
	for(int i=0; i<n; i++)
		for(int j=0; j<m; j++) V[i][j+p] = B[i][j];


	int rank = 0;
	int row = 0;

	vector<int> left(R, -1);

	//foward
	for(int col=0; col<C && row<R; col++){
		//pivot
		T val = abs( V[row][col] );
		int pivot = row;
		for(int j=row+1; j<R; j++){
			if(val < abs( V[j][col] )){
				val = abs( V[j][col] );
				pivot = j;
			}
		}
		if(pivot != row) swap(V[row], V[pivot]);

		if(val == 0) continue;

		T inv = mod_inverse(val, mod);
		for(int j=row+1; j<R; j++){
			T c = (V[j][col] * inv + mod) % mod;
			for(int k=col; k<p+m; k++) V[j][k] = ((V[j][k] - V[row][k] * c) % mod + mod) % mod;
		}

		left[row] = col;
		row++;
		rank++;
	}

	return rank;

	//backward
	for(int i=R-1; i>=0; i--){
		bool zero = true;
		bool valid = true;
		for(int col=0; col<p; col++) if(V[i][col] != 0) zero = false;
		for(int col = p; zero && col<C; col++) if(V[i][col] != 0) valid = false;

		if(valid == false) return -1;	//no solution

		if(left[i] == -1) continue;

		T inv = mod_inverse(V[i][ left[i] ], mod);
		for(int j=left[i]; j<C; j++) V[i][j] = (V[i][j] * inv) % mod;

		for(int j=i-1; j>=0; j--){
			for(int k=0; k<C; k++){
				V[j][k] = ( (V[j][k] - V[j][i] * V[i][k]) % mod + mod ) % mod;
			}
		}
	}
	//return V;
	return rank;
}
