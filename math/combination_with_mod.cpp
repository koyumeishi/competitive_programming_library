#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <functional>

using namespace std;

#define MOD 1000000007

//p is mod

//n! mod p table
long long fact[400000+1];

void make_fact(long long p){
	fact[0] = 1;
	for(int i=1; i<=400000; i++){
		fact[i] = ((long long)fact[i-1]*i%p)%p;
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

//nCk mod p
long long mod_comb(long long n, long long k, long long p){
	if(n<0 || k<0 || n<k) return 0;
	long long e1,e2,e3;
	long long a1 = mod_fact(n,p,e1);
	long long a2 = mod_fact(k,p,e2);
	long long a3 = mod_fact(n-k,p,e3);

	if(e1 > e2+e3) return 0;
	return a1 * mod_inverse(a2*a3 %p, p) %p;
}

//usage
{
	//first of all, make n!%MOD table with make_fact(MOD)
	make_fact(MOD);

	//secondly, calculate nCk % MOD
	long long c = mod_comb(R+N-1,N-1,MOD);
}
