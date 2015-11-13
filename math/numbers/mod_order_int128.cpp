/*

a^k = 1 (mod b)
となる最小の正整数kを求める
O(sqrt(b))

*/

#include <set>
#include <algorithm>
#include <cstdio>
using namespace std;

__int128 power_mod(__int128 x, __int128 y, __int128 mod){
	__int128 ret = 1;
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
__int128 baby_step_giant_step(__int128 x, __int128 y, long long z){
	__int128 H = sqrt(z) + 1;
	set<pair<long long,__int128>> S;

	for(__int128 i=0, w=y; i<H; i++, w=(w*x)%z){
		S.insert({w,i});
	}

	__int128 k = -1;
	__int128 x_H = power_mod(x, H, z);
	for(__int128 i=1, w=x_H; i<=H; i++, w=(w*x_H)%z){
		auto itr = S.lower_bound({w+1, 0});
		if(itr == S.begin()) continue;

		itr--;
		if(itr->first == w){
			return H * i - itr->second;

		}
	}

	return k;
}

__int128 get_order(__int128 a, __int128 b){
	return baby_step_giant_step(a, 1, b);
}

#include <iostream>

int main(){
	cerr << "get k such that a^k = 1 mod b" << endl;
	cerr << "input a b" << endl;
	long long a,b;
	cin >> a >> b;

	cout << (long long)get_order(a,b) << endl;
/*
	__int128 a_ = 1, b_ = b;
	int i = 0;
	for(i=1; i<=100000000; i++){
		a_ = (a_*a)%b_;
		if(a_ == 1) break;
	}
	cout << i << " " << (long long)a_ << endl;
*/
	return 0;
}