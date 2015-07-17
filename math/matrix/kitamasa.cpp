#include <iostream>
#include <vector>
#include <cstdio>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>
#include <queue>
#include <cmath>
#include <set>
using namespace std;

/*
x_k = Σ a_i * x_(k-d+i) で表される線形差分多項式の　第k項を求めてくれるえらいやつ

n*nのコンパニオン行列のk乗をO(n^2 log k)で求めて
x_k = Σ b_i * x_i の形にして計算する

*/

#define MOD 1000000007

template<class T>
class kitamasa{
public:
	int max_n;
	vector<T> A;
	vector<T> X;

	kitamasa(vector<T>& a, vector<T>& x){
		for(int i=0; i<a.size(); i++){ A.push_back(a[i]); }
		//X.push_back(0);
		for(int i=0; i<x.size(); i++){ X.push_back(x[i]); }
	}

	//ΣΣ Pi Qj X(i+j) -> Σ Ri Xi ( i = 0...d )
	vector<T> func(const vector<T>& P, const vector<T>& Q){
		int d = A.size();
		vector<T> tmp(2*d-1, 0);
		for(int i=0; i<d; ++i){
			for(int j=0; j<d; ++j){
				tmp[i+j] += (P[i] * Q[j]);
				//if(tmp[i+j] >= MOD) tmp[i+j] %= MOD;
			}
		}

		for(int i=2*d-2; i>=d; --i){
			for(int j=0; j<d; ++j){
				tmp[i - d + j] += (tmp[i] * A[j]);
				//if(tmp[i - d + j] >= MOD) tmp[i - d + j] %= MOD;
			}
		}

		tmp.resize(d);
		return tmp;
	}

	T calc(T k){
		int d = A.size();

		int lg = 0;
		while((1LL<<lg) <= k) lg++;
		// X[2^n] = Σ B[n][i] X[i] (i = 0...d)
		vector<vector<T>> B(lg, vector<T>(d, 0));
		B[0][1] = 1;
		for(int i=0; i+1<lg; i++){
			B[i+1] = func(B[i], B[i]);
		}

		vector<T> res(d, 0);
		res[0] = 1;

		int r = 0;
		while(k){
			if(k & 1){
				res = func(res, B[r]);
			}
			k>>=1;
			r++;
		}

		T ret = 0;
		for(int i=0; i<d; i++){
			ret += (res[i] * X[i]);
			ret %= MOD;
		}
		return ret;
	}
};

