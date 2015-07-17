/*

a^k = 1 (mod b)
となる最小の正整数kを求める
O(sqrt(b))

*/

#include <set>
#include <algorithm>
using namespace std;

long long power_mod(long long x, long long y, long long mod){
	long long ret = 1;
	while(y){
		if(y&1) ret = (ret*x) % mod;
		x = (x*x) % mod;
		y >>= 1;
	}
	return ret;
}

//find k such that x^k = y (mod z)
//x and z are co-prime
//x^(aH-b) = y (mod z)
//x^aH = y * x^b (mod z)
long long baby_step_giant_step(long long x, long long y, long long z){
	long long H = sqrt(z) + 1;
	set<pair<int,long long>> S;

	for(long long i=0, w=y; i<H; i++, w=(w*x)%z){
		S.insert({w,i});
	}

	long long k = -1;
	long long x_H = power_mod(x, H, z);
	for(long long i=1, w=x_H; i<=H; i++, w=(w*x_H)%z){
		auto itr = S.lower_bound({w+1, 0});
		if(itr == S.begin()) continue;

		itr--;
		if(itr->first == w){
			return H * i - itr->second;

		}
	}

	return k;
}

long long get_order(long long a, long long b){
	return baby_step_giant_step(a, 1, b);
}

#include <iostream>

int main(){
	cerr << "get k such that a^k = 1 mod b" << endl;
	cerr << "input a b" << endl;
	long long a,b;
	cin >> a >> b;
	cout << get_order(a,b) << endl;
	return 0;
}