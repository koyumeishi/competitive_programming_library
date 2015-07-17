#include <cmath>
#include <vector>
#include <complex>

#include <algorithm>

#include "assert.h"

#include <iostream>

using namespace std;


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

long long mod_pow(long long x, long long y, long long MOD){
	if(x==0) return 0;
	long long ret=1LL;
	while(y>0LL){
		if(y&1LL) ret = (ret * x) % MOD;
		x = (x*x) % MOD;
		y >>= 1LL;
	}
	return ret;
}

/*
FFTで使う回転子をあるMOD上の原始根にすることで、整数での畳み込みを可能にする O(N log N)
まちがい)高速に畳み込むには 原始根^2の冪 = 1 (MOD P) とする必要がある
せいかい)高速に畳み込むには、回転子を MOD P 上で位数が 2の冪 となるような x とする必要がある。また、この位数はNTTする列より長くなくてはならない
実用的には 10^5 <= 2^k <= 10^6 となる  17 <= k <= 21 (131072 ~ 2097152) 辺りか

逆変換のときには 原始根^-1 (MOD P) を原始根として使えば良い
これを複数の MOD P_k について行い、畳み込んだ後に中国剰余定理を使えば lcm( mod P_k ) 未満の数を復元することができる

NTT( f[x] (mod P_k) ) -> F[x]
NTT( g[x] (mod P_k) ) -> G[x]

Inverse_NTT( F[x]*G[x] (mod P_k) ) -> f*g (mod P_k)

Chinese_Remainder_Theorem ( f*g[x] (mod P_k) , MOD Q ) -> f*g[x] (mod Q)


あるmod上で2^k回で一回転する単位回転子の例
[k=11]
1297 98609153
29803 98062337
179 97792001
9151 97202177
4133 96872449
8597 96503809
3359 94707713
17761 94586881
23531 94130177
20201 93822977
37201 93644801
569 93493249
20357 93462529
34159 93319169
7643 93214721
31817 93130753
35089 92631041
35353 92110849
36923 92049409
13691 91514881
33023 90474497
6619 90408961
8539 89014273
15193 88823809
14887 88686593
15061 87730177
35401 87164929
2083 87042049
12637 86468609
9613 86458369
9257 86358017
18133 86222849
21787 85948417
16477 85893121
14821 85456897
18521 85063681
13729 84901889
23473 84416513
28351 83783681
829 83650561
12917 83519489

[k=12]
16183 98377729
1009 97497089
5483 97472513
16553 96485377
12527 96460801
17099 95711233
10079 95354881

[k=13] (rot, mod)
20963 49635329
16417 49586177
2591 49520641
5381 49119233
2887 48971777
35863 48906241
13687 48701441
6133 47718401
383 47628289
17911 47497217
33353 47349761
25339 47308801
35677 47300609
19259 47112193
15193 46686209
1033 46522369
30661 45588481
37619 45408257
25121 44941313
12721 44769281
27017 44433409
5591 44285953
10333 43769857
28283 43745281
29167 43425793
19751 43155457
3779 42590209
31883 42557441
12601 42418177
2971 42016769
24359 41893889
2801 41803777
31079 41746433
431 41680897
2917 41500673
8887 41066497
2543 41041921
33053 40960001
34369 40148993
9133 40058881
23447 39960577
30091 39854081

[k=14]
24763 99500033
26249 99106817
14867 98992129
32381 98861057
1217 98746369
8849 98123777
6803 97615873
33013 96485377
19447 96010241
14411 95813633
9391 95354881
24203 94781441
25621 94486529
1229 93880321
17737 93847553
15199 93601793
25471 93192193
26951 93110273

[k = 15] (2^15 = 32768 = O(3*10^4))
9859 98861057
6427 97615873
11393 97320961
157 96010241
6701 95027201
6637 94863361
17299 94273537
2399 93880321
8329 93192193
6569 90603521
8861 89849857

[k = 17] (2^17 = 131072 = O(10^5))
8	786433
10	8257537
43	6684673
47	6946817
74	1179649
124	2752513


[k = 18] (2^18 = 262144 = O(2*10^5))
5	786433
79	7340033
127	5767169
593	8650753

[k = 20]
5	7340033

[k=21]
100291	23068673

*/


template<typename T = long long>
class Number_Theoretic_Transform {
	// return the vector of F[t] or f[x] where
	// F[t] = sum of { f[x] * exp(-j * theta * t * x) } in x = 0 .. N-1 (FFT)
	// f(x) = 1/N * sum of { F[t] * exp(j * theta * t * x) } in t = 0 .. N-1 (inverse FFT)
	// where theta = 2*PI / N
	// N == 2^k
	// use the rotater as (primitive-root of mod) ^ t in NTT, which is used as exp(j*theta)^t in FFT

	//事前に計算した 単位回転子 rotater (MOD mod 上での位数が2^kとなる数) を 引数に与える。
	//逆変換のときには rotater^-1 (MOD mod) を rotaterに与える

	vector< T > do_NTT(vector< T > A, bool inverse){
		int N = A.size();

		//bit reverse
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
		vector<T>& theta = (inverse?inv_theta_v:theta_v);

		T t = k-1;
		T half = theta[k-1];	//半回転

		for(int level = 1; level<k; level++){
			T W_n = theta[t];	//rotater ^ theta (MOD mod)
			T W = 1;							//rotater
			for(int x=0; x < (1<<(level-1)); x++){
				for(int y=x; y+dist < N; y += (dist<<1)){
					T tmp = A[y+dist]*W;
					if(tmp >= mod) tmp %= mod;

					A[y+dist] = A[y] + (tmp*half) % mod;
					if(A[y+dist] >= mod) A[y+dist] %= mod;

					A[y] = A[y] + tmp;
					if(A[y] + tmp >= mod) A[y] %= mod;
				}
				W = W*W_n;
				if(W>=mod) W%=mod;
			}
			dist <<= 1;
			t -= 1;
		}

		if(inverse){
			for(int i=0; i<N; i++){
				A[i] = z * A[i];
				if(A[i] >= mod) A[i] %= mod;
			}
		}
		

		return A;
	}

public:
	const T mod;
	const T rotater;
	const T inv_rotater;
	const T k;
	vector<T> theta_v;
	vector<T> inv_theta_v;
	const T z;

	Number_Theoretic_Transform(T mod_, T rotater_, T k_) : 
		mod(mod_), 
		rotater(rotater_), 
		k(k_), 
		inv_rotater(mod_inverse(rotater_, mod)),
		z(mod_inverse(1<<(k-1), mod)) // 1/Nを掛けるところなので N^-1 MOD modを掛けたいところだけど何故か (N/2)^-1 で上手く行く……
	{

		theta_v = vector<T>(k+1,1);
		theta_v[0] = rotater;
		for(int i=1; i<=k; i++){
			theta_v[i] = theta_v[i-1] * theta_v[i-1];
			if(theta_v[i] >= mod) theta_v[i] %= mod;
		}

		inv_theta_v = vector<T>(k+1,1);
		inv_theta_v[0] = inv_rotater;
		for(int i=1; i<=k; i++){
			inv_theta_v[i] = inv_theta_v[i-1] * inv_theta_v[i-1];
			if(inv_theta_v[i] >= mod) inv_theta_v[i] %= mod;
		}
		
		

	};

	vector< T > NTT(vector< T > A){
		return do_NTT(A, false);
	}

	vector< T > INTT(vector< T > A){
		return do_NTT(A, true);
	}

	// vector<double> C | C[i] = ΣA[i]B[size-1-j]
	vector<T> convolution(vector<T> &A, vector<T> &B){
		int n = A.size();

		assert(A.size() == B.size());
		assert( n == (1<<k) );	//Nは2^kである必要がある(事前にresize)

		auto A_NTT = NTT(A);
		auto B_NTT = NTT(B);

		for(int i=0; i<n; i++){
			A_NTT[i] *= B_NTT[i];
			if(A_NTT[i] >= mod) A_NTT[i] %= mod;
		}
		return INTT(A_NTT);
	}
};



long long gcd(long long a, long long b){
	if(b==0) return a;
	return gcd(b, a%b);
}

long long lcm(long long a, long long b){
	if(a<b) swap(a,b);
	if(b==1) return a;
	return a * (b/gcd(a,b));
}

// Z % Yi = Xi であるようなZを求める。Garnerのアルゴリズム O(N^2)
// 参考 http://techtipshoge.blogspot.jp/2015/02/blog-post_15.html
// http://yukicoder.me/problems/448
long long Chinese_Remainder_Theorem_Garner(vector<long long> x, vector<long long> y, long long MOD){
	int N = x.size();

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




// for test
/*
vector<long long> Eratosthenes(long long N){
	vector<bool> v(N+1, true);
	v[0] = v[1] = false;
	int sqN = sqrt(N);
	for(long long i=2; i<=sqN; i++){
		if(v[i] == false) continue;
		for(long long j=i*i; j<=N; j+=i){
			v[j] = false;
		}
	}
	vector<long long> Prime;
	for(int i=2; i<=N; i++){
		if(v[i]) Prime.push_back(i);
	}
	return Prime;
}
*/

//do Eratosthenes in O(N) time
vector<long long> Eratosthenes(int N){
	vector<int> v(N+1, 0);
	iota(v.begin()+2, v.end(), 2);
	vector<long long> Prime;
	for(int i=2; i<=N; ++i){
		if(v[i] == i) Prime.push_back(i);
		for(int j=0; j<Prime.size() && Prime[j]<=v[i]; j++){
			long long tmp = Prime[j] * i;
			if(tmp <= N) v[tmp] = Prime[j];
			else break;
		}
	}
	return Prime;
}


//位数が2^kのもの
vector<pair<long long, long long>> order(long long k){
	long long b = 1<<k;

	vector<long long> mod = Eratosthenes(100000000);
	reverse(mod.begin(), mod.end());
	vector<pair<long long, long long>> ret;

	for(long long v : mod){
		if(v<b) continue;
		for(long long a : vector<long long>(mod.rbegin(), mod.rbegin()+2000)) {
			if(a>v) continue;
			if(mod_pow(a,b,v) != 1) continue;
			if(mod_pow(a,b/2,v) == 1) continue;
			cerr << a << " " << v << endl;
			ret.push_back({a,v});
			break;
		}
	}
	return ret;
}



//test
int main(){

	vector<Number_Theoretic_Transform<long long>> NTT;
	vector<long long> mod = {257,120833,124673};
	vector<long long> rot = {3,204,245};
	for(int i=0; i<3; i++){
		NTT.push_back(Number_Theoretic_Transform<long long>(mod[i], rot[i], 8));
	}
	vector<long long> a(1<<6);
	iota(a.begin(), a.end(), 1);
	vector<long long> b = {1};
	a.resize(1<<8,0);
	b.resize(1<<8,0);

	vector<vector<long long>> c(3);
	for(int i=0; i<3; i++){
		c[i] = NTT[i].convolution(a,b);
		for(int j=0; j<c[i].size(); j++){
			cout << c[i][j] << " ";
		}cout << endl;
	}

	vector<long long> d(a.size());
	for(int i=0; i<a.size(); i++){
		vector<long long> v(3);
		for(int j=0; j<3; j++){
			v[j] = c[j][i];
		}

		d[i] = Chinese_Remainder_Theorem_Garner(v,mod, 10007);
		cout << d[i] << " ";
	}cout << endl;


	order(15);
	return 0;
}