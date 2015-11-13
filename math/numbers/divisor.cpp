
// number of divisor d(n)
// n = \prod {p_i}^{k_i}
// d(n) = \prod {k_i+1}
// O(sqrt(n))
long long number_of_divisor(long long n){
	long long ret = 1;
	for(long long k=2; k*k<=n; k++){
		if(n%k != 0) continue;
		long long cnt = 1;
		while(n%k == 0){
			cnt++;
			n/=k;
		}
		ret *= cnt;
	}
	if(n!=1){
		ret *= 2;
		n /= n;
	}
	return ret;
}


// sum of divisor s(n)
// n = \prod {p_i}^{k_i}
// s(n) = \prod \frac {{p_i}^{k_i}-1} {p_i - 1}
// O(sqrt(n))

long long sum_of_divisor(long long n){
	long long ret = 1;
	for(long long k=2; k*k<=n; k++){
		if(n%k != 0) continue;
		long long tmp = k;
		while(n%k == 0){
			tmp *= k;
			n/=k;
		}
		ret *= (tmp-1) / (k-1);
	}
	if(n!=1){
		ret *= n+1;
	}
	return ret;
}

double sum_of_divisor_(long long n){
	double ret = 1;
	for(long long k=2; k*k<=n; k++){
		if(n%k != 0) continue;
		double tmp = k;
		while(n%k == 0){
			tmp *= k;
			n/=k;
		}
		ret *= (tmp-1) / (k-1);
	}
	if(n!=1){
		long long k = n;
		double tmp = k;
		while(n%k == 0){
			tmp *= k;
			n/=k;
		}
		ret *= (tmp-1) / (k-1);
	}
	return ret;
}

#include <iostream>
#include <random>
#include <ctime>
using namespace std;
int main(){
	/*
	mt19937 mt((unsigned)time(NULL));
	uniform_int_distribution<long long> dstr(1, (long long)1e12);
	//long long val = 4601076480; //996478560
	for(int i=1; i<100000; i++){
		long long n = i;//dstr(mt);
		long long a = sum_of_divisor(n);
		double b = sum_of_divisor_(n);
		if(fabs(a-b) > 1){
			cerr << n << endl;
			cerr << a << " " << b << endl;
		}
	}
	*/
	
	long long n;
	cin >> n;
	cout << sum_of_divisor(n) << endl;
	cout << sum_of_divisor_(n) << endl;
	
}