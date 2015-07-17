#include <cmath>
#include <vector>

using namespace std;
//this doesn't work


//do Eratosthenes in O(N) time
vector<int> Eratosthenes_linear(int N){
	vector<int> v(N+1, 0);
	iota(v.begin()+2, v.end(), 2);
	vector<int> Prime;
	for(int i=2; i<=N; ++i){
		if(v[i] == i) Prime.push_back(i);
		for(int j=0; j<Prime.size() && Prime[j]<=f[i]; j++){
			long long tmp = Prime[j] * i;
			if(tmp <= N) v[tmp] = Prime[j];
			else break;
		}
	}
	return Prime;
}
