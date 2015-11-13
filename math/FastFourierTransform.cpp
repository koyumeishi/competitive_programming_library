#include <cmath>
#include <vector>
#include <complex>

#include <algorithm>



#include <iostream>

using namespace std;

// http://d.hatena.ne.jp/nurs/20130617/1371483633


template<typename T = double>
class Fast_Fourier_Transform{
	// return the vector of F[t] or f[x] where
	// F[t] = sum of { f[x] * exp(-j * theta * t * x) } in x = 0 .. N-1 (FFT)
	// f(x) = 1/N * sum of { F[t] * exp(j * theta * t * x) } in t = 0 .. N-1 (inverse FFT)
	// where theta = 2*PI / N
	// N == 2^k
	static vector< complex<T> > do_fft(vector< complex<T> > A, bool inverse){
		const T PI = atan2(1, 0) * 2.0;
		const complex<T> j = complex<T>(0,1); // 0 + j*1.0 (j is the imaginary unit)

		int N = A.size();
		int k = 0;	// N = 2^k
		while(N>>k > 0) k++;
		
		for(int bit=0; bit<N; bit++){
			int rbit = 0;
			for(int i=0, tmp_bit = bit; i<k-1; i++, rbit <<= 1, tmp_bit >>=  1){
				rbit |= tmp_bit & 1;
			}
			rbit >>= 1;
			if(bit < rbit){
				swap(A[bit], A[rbit]);
			}
		}

		int dist = 1;
		T theta = -PI;
		if(inverse) theta = -theta;

		for(int level = 1; level<k; level++){
			complex<T> W_n = exp(j * theta);
			complex<T> W(1,0);
			for(int x=0; x < (1<<level-1); x++){
				for(int y=x; y+dist < N; y += (dist<<1)){
					complex<T> tmp = A[ y+dist ] * W;
					A[ y+dist ] = A[ y ] - tmp;
					A[ y ] += tmp;
				}
				W *= W_n;
			}
			dist <<= 1;
			theta *= 0.5;
		}

		if(inverse){
			T k = 1.0/N;
			for(int i=0; i<N; i++){
				A[i] *= k;
			}
		}
		return A;
	}


	template<typename U=T>
	static void vec_resize(vector<U> &A, const U val){
		int n = A.size();
		int k = 1;
		while(n > k) k<<=1;
		A.resize(k, val);
	}

public:
	Fast_Fourier_Transform(){};

	static vector< complex<T> > FFT(vector< complex<T> > A){
		vec_resize<complex<T>>(A, complex<T>(0,0));
		return do_fft(A, false);
	}

	static vector< complex<T> > IFFT(vector< complex<T> > A){
		//vec_resize<complex<T>>(A, complex<T>(0,0));
		return do_fft(A, true);
	}

	static vector< complex<T> > FFT(vector<T> A){
		vec_resize<T>(A, 0);
		vector<complex<T>> B(A.size());
		for(int i=0; i<B.size(); i++){
			B[i] = complex<T>(A[i], 0);
		}

		return do_fft(B, false);
	}

	static vector< complex<T> > FFT(vector<int> A){
		vec_resize<int>(A, T(0));
		vector<complex<T>> B(A.size());
		for(int i=0; i<B.size(); i++){
			B[i] = complex<T>(A[i], 0);
		}
		return do_fft(B, false);
	}

	static vector<int> round(vector<complex<T>> &&A){
		vector<int> ret(A.size());
		for(int i=0; i<A.size(); i++){
			ret[i] = A[i].real() + (A[i].real()<0?-0.5:0.5);
		}
		return ret;
	}

	// vector<double> C | C[i] = ΣA[i]B[j]
	static vector<complex<T>> convolution(vector<T> &A, vector<T> &B){
		reverse(B.begin(), B.end());

		int n = max(A.size(), B.size());
		A.resize(n, 0);
		B.resize(n, 0);

		auto A_FFT = FFT(A);
		auto B_FFT = FFT(B);
		for(int i=0; i<n; i++){
			A_FFT[i] *= B_FFT[i];
		}
		return IFFT(A_FFT);
	}
};

//test
int main(){

	//vector<double> A = {1,2,3,4,0,0,0,0};
	//vector<double> B = {1,10,100};

	vector<double> A = {6,4,5,1,3,2,6,4,5,1,3,2};
	vector<double> B = {1,1,1};
	int k=1;
	while(A.size() >= k) k<<=1;
	A.resize(k,0);


	auto print = [&](vector<complex<double>> A){
		for(int i=0; i<A.size(); i++){
			cout << (int)(A[i].real() + 0.5) << " ";
		}
		cout << endl;
	};

	auto C = Fast_Fourier_Transform<>::round(Fast_Fourier_Transform<>::convolution(A,B));

	for(int i=0; i<C.size(); i++){
		cout << C[i] << " ";
	}
	cout << endl;
	
	return 0;
}