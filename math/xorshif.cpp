#include <cstdint>


using namespace std;

class XorShift{
public:
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t w;

	XorShift(){
		x = 123456789;
		y = 362436069;
		z = 521288629;
		w = 88675123;
	}
	
	XorShift(uint32_t seed){
		x = 123456789;
		y = 362436069;
		z = 521288629;
		w = seed;

		for(int i=0; i<seed%20; i++){
			xor128();
		}
	}
	
	uint32_t xor128(void) {
		uint32_t t = x ^ (x << 11);
		
		x = y; y = z; z = w;
		return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)); 
	}
};

XorShift x_rand;

#include <iostream>
//test
int main(){
	int n;
	cin >> n;

	x_rand = XorShift((uint32_t)(&n));

	for(int i=0; i<n; i++){
		cout << rand.xor128() << endl;
	}


}