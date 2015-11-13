//
// N = p1 ^ k1 * p2 ^ k2 * ... * pn ^ kn
// number of divisors = (k1+1)*(k2+1)* ... * (kn+1)
//

long long number_of_factors(long long N){
	long long ret = 1;
	for(long long i=2; i*i<=N; i++){
		long long tmp = 0;
		while(N%i==0){
			tmp++;
			N/=i;
		}
		ret *= tmp+1;
	}
	if(N!=1){
		ret *= 2;
	}
	return ret;
}

#include <vector>
#include <map>
vector<pair<long long, long long> > prime_factorization(long long N){
	vector< pair<long long, long long> > ret;
	
	for(long long i=2; i*i<=N; i++){
		long long tmp = 0;
		while(N%i==0){
			tmp++;
			N/=i;
		}
		if(tmp>0){
			ret.push_back( make_pair(i, tmp) );
		}
	}
	if(N!=1){
		ret.push_back( make_pair(N, 1) );
	}
	return ret;
}

vector<long long> divisors(long long N){
	vector<long long> ret;
	ret.push_back(1);
	ret.push_back(N);

	for(long long i=2; i*i<=N; i++){
		if(n%i==0){
			ret.push_back(i);
			if(i*i != N) ret.push_back(N/i);
		}
	}

	//sort(ret.begin(), ret.end());
	return ret;
}