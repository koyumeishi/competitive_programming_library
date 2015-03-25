#include <cmath>
#include <vector>

using namespace std;

vector<int> Eratosthenes(int N){
	vector<bool> v(N+1, true);
	v[0] = v[1] = false;
	int sqN = sqrt(N);
	for(int i=2; i<=sqN; i++){
		if(v[i] == false) continue;
		for(int j=i*i; j<=N; j+=i){
			v[j] = false;
		}
	}
	vector<int> Prime;
	for(int i=2; i<=N; i++){
		if(v[i]==true) Prime.push_back(i);
	}
	return Prime;
}

#include <iostream>

int main(){
	int N;
	cin >> N;
	auto p = Eratosthenes(N);
	for(auto i : p){
		cout << i << endl;
	}
	return 0;
}