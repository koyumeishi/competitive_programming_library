#include <vector>
#include <array>

#include <iostream>
using namespace std;

// a0 x0 + a1 x1 + ... + an-1 xn-1
template<class T=int>
class Polynomial{
public:
	const int size;
	vector<T> a;
	Polynomial(int size_) : size(size_), a(size_){
	}
	Polynomial(const Polynomial<T>& x) : size(x.size){
		a = x.a;
	}

	Polynomial<T> operator = (const Polynomial<T>& x){
		a = x.a;
		return *this;
	}

	T& operator [] (const int& index){
		return a[index];
	}

	T operator [] (const int& index) const {
		return a[index];
	}


	Polynomial<T> operator + (const Polynomial<T>& x){
		Polynomial<T> ret = *this;
		for(int i=0; i<size; i++){
			ret[i] += x[i];
		}
		return ret;
	}

	Polynomial<T> operator - (const Polynomial<T>& x){
		Polynomial<T> ret = *this;
		for(int i=0; i<size; i++){
			ret[i] -= x[i];
		}
		return ret;
	}

	Polynomial<T> operator * (const T& x){
		Polynomial<T> ret = *this;
		for(int i=0; i<size; i++){
			ret[i] *= x;
		}
		return ret;
	}

	T operator () (const vector<T>& x){
		T ret(0);
		for(int i=0; i<size; i++){
			ret += a[i] * x[i];
		}
		return ret;
	}

};

ostream& operator << (ostream& os, const Polynomial<int>& poly){
	for(int i=0; i<poly.size; i++){
		os << poly[i] << " ";
	}
	return os;
}

int main(){
	int n;
	cin >> n;
	vector<int> x(n);
	for(int i=0; i<n; i++){
		cin >> x[i];
	}

	vector<Polynomial<int>> p(n, Polynomial<int>(n));
	for(int i=0; i<n; i++){
		p[i][i] = 1;
		cerr << p[i] << endl;
	}cerr << endl;

	for(int i=0; i<n; i++){
		vector<Polynomial<int>> p_(n, Polynomial<int>(n));
		for(int j=0; j<n; j++){
			p_[j] = p[j] - (j>0?p_[j-1]:Polynomial<int>(n));
		}
		for(int j=0; j<n; j++){
			cerr << p_[j] << p_[j](x) << endl;
		}
		cerr << endl;
		swap(p, p_);
	}

}