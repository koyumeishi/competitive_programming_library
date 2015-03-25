vector<int> multmat(vector<int> &A, vector<int> &B, int n, int p, int m){
	vector<int> C(n*m, 0);
	for(int i=0; i<n; i++){
		for(int k=0; k<p; k++){
			for(int j=0; j<m; j++){
				C[i*m + j] += A[i*p + k] * B[k*m + j];
				//C[i*m + j] %= mod;
			}
		}
	}
	return C;
}

//[n*p] * [p*m] => [n*m]
template<class T>
vector< vector<T> > multmat(const vector<vector<T> > &A, const vector<vector<T>> &B, int n, int p, int m){
	vector<vector<T> > C(n, vector<T>(m,0));
	for(int i=0; i<n; i++){
		for(int k=0; k<p; k++){
			for(int j=0; j<m; j++){
				C[i][j] += A[i][k] * B[k][j];
				//C[i][j] %= mod;
			}
		}
	}
	return C;
}

//A[n*n]^k 
template<class T>
vector< vector<T> > mat_pow(vector<vector<T> > A, int k){
	int n = A.size();
	vector<vector<T> > ret(n, vector<T>(n, 0) );
	for(int i=0; i<n; i++){
		ret[i][i] = 1;
	}
	while(k>0){
		if(k&1) ret = multmat(A,ret, n,n,n);
		A = multmat(A,A, n,n,n);
		k>>=1;
	}
	return ret;
}