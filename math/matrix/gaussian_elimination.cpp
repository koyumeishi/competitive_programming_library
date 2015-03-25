#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

void print(const vector<vector<double> > &v){
	int n = v.size();
	int m = v[0].size();
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			cerr << v[i][j] << (j==(n-1)? " | " : " ");
		}
		cerr << endl;
	}
	
}


//A[n*p]*b[p*1] = x[n*1]
vector<vector<double> > gaussian_elimination(vector<vector<double> > &A, vector<vector<double> > &x){
	bool valid = true;
	int rank=0;
	int n = A.size();
	int p = A[0].size();
	vector< vector<double> > R = A;
	for(int i=0; i<n; i++){
		R[i].insert(R[i].end(), x[i].begin(), x[i].end());
	}
	int m = R[0].size();	//p + x[0].size()

	print(R);

	//foward
	//row
	for(int i=0; i<n; i++){

		//pivot
		int pivot_row = i;
		double pivot_val = fabs(R[i][i]);
		//choose pivot row
		for(int j=i+1; j<n; j++){
			if(pivot_val < fabs(R[j][i])){
				pivot_row = j;
				pivot_val = fabs(R[j][i]);
			}
		}
		if(pivot_row != i){
			swap(R[i], R[pivot_row]);
		}

		if(pivot_val <= 1e-9){
			
			continue;
		}

		rank++;

		{
			double divisor = 1.0/R[i][i];
			//A[i][j] == 0, such that (0 <= j < i)
			for(int j=i; j<m; j++){
				R[i][j] *= divisor;
			}
		}
		
		for(int j=i+1; j<n; j++){
			double divisor = R[j][i];	// R[j][i]/R[i][i], but R[i][i] is 1.0
			for(int k=i; k<m; k++){
				R[j][k] -= R[i][k] * divisor;
			}
		}
	}

	//backward
	//row
	for(int i=n-1; i>=0; i--){
		for(int j=i-1; j>=0; j--){
			double c = R[j][i];
			for(int k=i; k<m; k++){
				R[j][k] -= R[i][k] * c;
			}
		}
	}

	for(int i=0; i<n; i++){
		bool row_valid = false;
		for(int j=i; j<m; j++){
			if(fabs(R[i][j]) <= 1e-9){
				R[i][j] = 0.0;
			}else if(j<p){
				row_valid = true;
			}
		}
		if(row_valid == false && R[i][n] != 0.0){
			valid = false;
		}
	}
	print(R);
	cerr << "rank is " << rank << endl;
	cerr << (valid?"valid":"invalid, no solutions") << endl;

	return R;
}


int main(){
	int n;
	cin >> n;
	vector<vector<double> > A(n, vector<double>(n, 0));
	for(int i=0; i<n; i++){
		for(int j=0; j<n; j++){
			cin >> A[i][j];
		}
	}

	vector<vector<double> > b(n,vector<double>(1,0));
	for(int i=0; i<n; i++){
		cin >> b[i][0];
	}

	vector<vector<double> > res = gaussian_elimination(A, b);

	return 0;
}