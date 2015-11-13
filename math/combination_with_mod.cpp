#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <functional>

using namespace std;


// nCk mod p, O(1)
// precomputation O(size)
class combination_mod{
	const long long mod;
	const long long size;
	
	vector<long long> fact;	//n!
	vector<long long> fact_inv;	// (n!)^-1

	void make_fact(){
		fact[0] = 1;
		for(long long i=1; i<size; i++){
			fact[i] = fact[i-1]*i % mod;
		}
	}

	void make_fact_inv(){
		fact_inv[0] = fact_inv[1] = 1;
		for(long long i=2; i<size; i++){
			fact_inv[i] = fact_inv[mod%i] * (mod - mod/i) % mod;	// x ^ -1
		}
		for(int i=2; i<size; i++){
			fact_inv[i] = fact_inv[i-1] * fact_inv[i] % mod;	// x! ^ -1
		}
	}

public:
	combination_mod(long long mod_, long long size_ = 2000000) : mod(mod_), size(size_+1){
		fact.resize(size);
		fact_inv.resize(size);
		make_fact();
		make_fact_inv();
	}

	//nCk mod p O(1)
	long long comb(long long n, long long k){
		if(k==0 || n==k) return 1;
		long long ret = fact[n] * fact_inv[k] % mod * fact_inv[n-k] % mod;
		return ret;
	}
};


///////////////////////////////////////////////////////////////////////////
// old ver
//p is mod

//n! mod p table
long long fact[400000+1];


void make_fact(long long mod){
	fact[0] = 1;
	for(long long i=1; i<=400000; i++){
		fact[i] = ((long long)fact[i-1]*i)%mod;
	}
}

long long fact_inv[400000+1];

void make_fact_inv(long long mod){
	fact_inv[0] = fact_inv[1] = 1;
	for(long long i=2; i<=400000; i++){
		fact_inv[i] = fact_inv[mod%i] * (mod - mod/i) % mod;
	}
	for(int i=2; i<=400000; i++){
		fact_inv[i] = fact_inv[i-1] * fact_inv[i] % mod;
	}
}

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

long long mod_fact(long long n, long long p, long long &e){
	e = 0;
	if(n==0) return 1;

	long long res = mod_fact(n/p, p, e);
	e += n/p;

	if(n/p %2 != 0) return res * (p-fact[n%p]) %p;
	return res * fact[n%p]%p;
}

//nCk mod p O(log n)
long long mod_comb(long long n, long long k, long long p){
	if(n<0 || k<0 || n<k) return 0;
	long long e1,e2,e3;
	long long a1 = mod_fact(n,p,e1);
	long long a2 = mod_fact(k,p,e2);
	long long a3 = mod_fact(n-k,p,e3);

	if(e1 > e2+e3) return 0;
	return a1 * mod_inverse(a2*a3 %p, p) %p;
}

//nCk mod p O(1)
long long mod_comb_(long long n, long long k, long long p){
	if(k==0 || n==k) return 1;
	long long ret = fact[n] * fact_inv[k] % p * fact_inv[n-k] % p;
	return ret;
}

//usage
{
	//first of all, make n!%MOD table with make_fact(MOD)
	make_fact(MOD);

	//secondly, calculate nCk % MOD
	long long c = mod_comb(R+N-1,N-1,MOD);
}
