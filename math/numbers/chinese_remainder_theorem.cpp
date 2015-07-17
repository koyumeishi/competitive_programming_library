#include <vector>
#include <map>
using namespace std;

long long gcd(long long a, long long b){
	if(b==0) return a;
	return gcd(b, a%b);
}

long long lcm(long long a, long long b){
	if(a<b) swap(a,b);
	if(b==1) return a;
	return a * (b/gcd(a,b));
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


// return x where x mod p_i = value_i
// v[i] = pair{ value_i , p_i }
long long chinese_remainder_theorem(const vector<pair<long long,long long>>& v){
	long long M = 1;
	for(int i=0; i<v.size(); i++){
		M = lcm(M,v[i].second);
	}

	long long ret = 0;
	for(int i=0; i<v.size(); i++){
		long long M_i = (M/v[i].second);
		ret += (v[i].first * M_i * mod_inverse( M_i%v[i].second, v[i].second ))%M;
		ret = ret%M;
	}
	return ret%M;
}

// return x where x mod p_i = value_i
// v[i] = pair{ value_i , p_i }
long long chinese_remainder_theorem(const vector<long long>& value, const vector<long long>& mod){
	vector<pair<long long,long long>> v(value.size());
	for(int i=0; i<v.size(); i++){
		v[i] = {value[i], mod[i]};
	}
	return chinese_remainder_theorem(v);
}

#include <iostream>
#include <cmath>
int main(){
	// n! mod m
	long long n,m;
	cin >> n >> m;
	vector<long long> prime;
	long long ub = sqrt(m);
	for(long long i=2; i<=ub; i++){
		if(m%i != 0) continue;
		long long tmp = i;
		while(m % (tmp*i) == 0) tmp *= i;
		prime.push_back(tmp);
		m /= tmp;
	}
	if(m != 1){
		prime.push_back(m);
	}

	vector<long long> factorial(prime.size(), 1);
	for(int i=1; i<=n; i++){
		for(int j=0; j<prime.size(); j++){
			factorial[j] = (factorial[j] * i) % prime[j];
		}
	}
	cout << chinese_remainder_theorem(factorial, prime) << endl;
}


// Z % Yi = Xi であるようなZを求める。Garnerのアルゴリズム O(N^2)
// 参考 http://techtipshoge.blogspot.jp/2015/02/blog-post_15.html
// http://yukicoder.me/problems/448
long long Chinese_Remainder_Theorem_Garner(vector<long long> x, vector<long long> y){

	bool valid = true;
	//前処理
	//gcd(Yi,Yj) == 1 (i!=j) でなくてはならないので、
	//共通の因数 g = gcd(Yi,Yj) を見つけたら片側に寄せてしまう
	for(int i=0; i<N; i++){
		for(int j=i+1; j<N; j++){
			if(i == j) continue;
			long long g = gcd(y[i], y[j]);

			if( x[i]%g != x[j]%g ) valid = false;	//解が存在しない

			if(g != 1){
				y[i] /= g; y[j] /= g;
				long long g_ = gcd(y[i], g);
				while(g_ != 1){
					y[i] *= g_;
					g /= g_;
					g_ = gcd(y[i], g);
				}
				y[j] *= g;

				x[i] %= y[i];
				x[j] %= y[j];
			}
		}
	}

	if(!valid){
		cerr << -1 << endl;
		return 0;
	}

	//Garner's algorithm
	vector<long long> z(N);
	for(int i=0; i<N; i++){
		z[i] = x[i];
		for(int j=0; j<i; j++){
			z[i] = mod_inverse(y[j], y[i]) % y[i] * (z[i] - z[j]) % y[i];

			z[i] = (z[i]+y[i])%y[i];
		}
	}

	long long ans = 0;
	long long tmp = 1;
	for(int i=0; i<N; i++){
		ans = (ans + z[i] * tmp)%MOD;
		tmp = (tmp * y[i])%MOD;
	}

	return ans;
}