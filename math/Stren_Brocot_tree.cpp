#include <iostream>
#include <algorithm>
#include <functional>
#include <cstdio>

using namespace std;

class Stern_Brocot_tree{
	int LIMIT;

	// n / d
	struct Rational{
		int n;
		int d;
		Rational(int n_, int d_) : n(n_), d(d_){
		}

		Rational operator+(const Rational &x) const{
			return Rational(this->n + x.n, this->d + x.d);
		}
		
		void print(){
			printf("%d/%d ", n,d);
		}
	};

	bool breaker(const Rational &x){
		if(x.n+x.d > LIMIT) return true;
		return false;
	}
	bool breaker(const int &depth){
		if(depth > LIMIT) return true;
		return false;
	}

	void rec(const Rational &low = Rational(0,1), const Rational &high = Rational(1,0), const int depth = 0){
		Rational med = low+high;

		//if(breaker(med)) return;
		if(depth>LIMIT) return;

		rec(low, med, depth+1);

		med.print();

		rec(med, high, depth+1);

	}

public:

	Stern_Brocot_tree(int limit = 1000){
		LIMIT = limit;
	}


	void calc(int high_n=1, int high_d=0){
		rec(Rational(0,1), Rational(high_n, high_d));
	}

};

int main(){
	Stern_Brocot_tree sbt(3);
	sbt.calc(1,1);
	return 0;
}