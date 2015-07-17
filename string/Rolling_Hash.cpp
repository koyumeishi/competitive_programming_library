#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// 13^1000000008 = 1 mod 1000000009
// 15^1000000008 = 1 mod 1000000009
// 78^1000000008 = 1 mod 1000000009


// 13^1000000006 = 1 mod 1000000007
// 15^1000000006 = 1 mod 1000000007
// 78^1000000006 = 1 mod 1000000007

//Hash[i] = Σ(k=0..i) s_k * P^i-k-1

class Rolling_Hash{
	int N;
	vector<int> hash;
	vector<int> p_pow;
	const int P;
	const int MOD;

	void constract(const string& s){
		//hash[i] = [0,i)
		for(int i=0; i<N; i++){
			hash[i+1] = (1LL * hash[i] * P + s[i])%MOD;
		}
	}
public:
	Rolling_Hash(const string& s) : 
		P(vector<int>{10009, 10007}[0/*rand()%2*/]),
		MOD(vector<int>{1000000007, 1000000009}[0/*rand()%2*/]),
		N(s.size()),
		hash(s.size()+1, 0),
		p_pow(s.size()+1)
	{
		constract(s);
		p_pow[0] = 1;
		for(int i=1; i<=N; i++){
			p_pow[i] = (1LL * p_pow[i-1] * P) % MOD;
		}
	}

	// [l,r)
	long long get_hash(int l, int r){
		int len = r-l;
		return (hash[r] - (1LL * hash[l]* p_pow[len])%MOD + MOD) % MOD;
	}
};

#include <iostream>
int main(){
	string s;
	cin >> s;
	Rolling_Hash rh(s);
	int l, r;
	while(cin >> l >> r) cout << rh.get_hash(l,r) << endl;
	return 0;
}