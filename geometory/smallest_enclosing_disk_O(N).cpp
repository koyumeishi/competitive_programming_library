// using class xy, class xorshift


#include <vector>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cstdio>
#include <ctime>
#include <functional>

using namespace std;

#define EPS 1e-6

class xy{
	
public:
	double x;
	double y;

	xy(){

	}
	
	xy(double xx, double yy){
		x = xx;
		y = yy;
	}

	xy(const xy &v){
		x = v.x;
		y = v.y;
	}

	xy& operator=(const xy &v){
		x = v.x;
		y = v.y;
		return *this;
	}

	xy operator+(const xy &v) const{
		return xy(this->x+v.x, this->y+v.y);
		
	}
	
	xy operator-(const xy &v) const{
		return xy(this->x-v.x, this->y-v.y);
	}
	
	void operator+=(const xy &v){
		x+=v.x;
		y+=v.y;
	}
	void operator-=(const xy &v){
		x-=v.x;
		y-=v.y;
	}
/* something wrong this operator for sorting
	bool operator<(const xy &v){
		if(x!=v.x) return x < v.x;
		return y < v.y;
	}
	bool operator>(const xy &v){
		if(x!=v.x) return x > v.x;
		return y > v.y;
	}
*/

};
//for sorting
bool comp_xy(const xy &a, const xy &b){
	if(a.x != b.x) return a.x < b.x;
	return a.y < b.y;
}

//u,v : vector O = (0,0)
double cross(const xy &u, const xy &v){
	return u.x*v.y - u.y*v.x;
}

//u,v : vector O = (0,0)
double dot(const xy &u, const xy &v){
	return u.x*v.x + u.y*v.y;
}

//distance between two points
double dist_p_p(const xy &a, const xy &b){
	return sqrt( fabs(dot(a-b, a-b)) );
}

//distance between a point and a line segment
double dist_p_ls(const xy &p, const xy &s1, const xy &s2){
	xy vl = s2 - s1;
	xy vp = p - s1;

	return fabs( cross(vl, vp) / sqrt( dot(vl, vl) ) );
}

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


//Welzl's algorithm
//O(N)

// CAUTION
//	this is heuristic algorithm

//http://web.nmsu.edu/~xiumin/project/smallest_enclosing_circle/


//Welzl's algorithm?
//O(N)
class Smallest_Enclosing_Disk{

	typedef struct{
		double r = 0.0;
		xy O = xy(0,0);
	}disk;
	
	void make_disk_from_points(const int &P, disk &ret){
		ret.r = 0.0;
		ret.O = v[P];
	}
	
	void make_disk_from_points(const int &P, const int &Q, disk &ret){
		//disk ret;
		if(v[P].x == v[Q].x && v[P].y == v[Q].y){
			ret.r = 0.0;
			ret.O = v[P];
			return;
		}
		ret.O.x = (v[P].x + v[Q].x) * 0.5;
		ret.O.y = (v[P].y + v[Q].y) * 0.5;
		ret.r = dist_p_p(v[P], v[Q]) * 0.5;
		//return ret;
	}

	void make_disk_from_points(const int &P, const int &Q, const int &R, disk &ret){
		//disk ret;
		
		if(dot(v[Q] - v[P], v[R] - v[P]) <= 0 + EPS){
			make_disk_from_points(Q, R, ret);
			return;
			
		}else if(dot(v[P] - v[Q], v[R] - v[Q]) <= 0 + EPS){
			make_disk_from_points(P, R, ret);
			return;
			
		}else if(dot(v[P] - v[R], v[Q] - v[R]) <= 0 + EPS){
			make_disk_from_points(P, Q, ret);
			return;
			
		}else{
			xy a = v[R] - v[Q];
			xy b = v[R] - v[P];
			xy c = v[Q] - v[P];
			double A = dot(a, a);
			double B = dot(b, b);
			double C = dot(c, c);

			//r(x,y) = { A(B+C-A)r_a + B(C+A-B)r_b + C(A+B-C)r_c } / 16S^2
			//16S^2 = A(B+C-A) + B(C+A-B) + C(A+B-C)

			double S_ = A*(B+C-A) + B*(C+A-B) + C*(A+B-C);
			ret.O.x = (A*(B+C-A) * v[P].x + B*(C+A-B) * v[Q].x + C*(A+B-C) * v[R].x) / S_;
			ret.O.y = (A*(B+C-A) * v[P].y + B*(C+A-B) * v[Q].y + C*(A+B-C) * v[R].y) / S_;
			ret.r = dist_p_p( v[P], ret.O );
		}

		//return ret;
	}
	

	//P := points
	//B := points on the boundary
	disk getDisk(const vector<int> &P, int n, vector<int> &B, int k){
		disk ret;
		ret.r = 0.0;
		ret.O = xy(0,0);
		
		if(k == 1){
			make_disk_from_points(B[0], ret);
		}else if(k == 2){
			make_disk_from_points(B[0],B[1], ret);
		}else if(k == 3){
			make_disk_from_points(B[0],B[1],B[2], ret);
		}
		if(k != 3){
			for(int i=0; i<n; i++){
				//P[i] is outside of the disk
				double dist = dist_p_p(v[P[i]], ret.O);
				if( dist > ret.r + EPS){
					B[k] = P[i];
					ret = getDisk(P,i,B,k+1);
				}
			}
		}
			
		return ret;
	}

public:
	vector<xy> v;
	
	//r, xy
	Smallest_Enclosing_Disk(const int n){
		v.resize(n);
	}
	Smallest_Enclosing_Disk(const vector<xy> &v){
		this->v = v;
	}

	static int myshuffle(int i){
		int ret = x_rand.xor128() % (unsigned int)i;
		return ret;
	}
	
	double calc(){
		int n = v.size();
		vector<int> P(n);
		for(int i=0; i<P.size(); i++) P[i] = i;
		disk ret;
		vector<int> B(3);
		while(1){
			random_shuffle(P.begin(), P.end(), Smallest_Enclosing_Disk::myshuffle);
			ret = getDisk(P, n, B, 0);
			bool valid = true;;
			for(int i=0; i<n; i++){
				if( dist_p_p( ret.O, v[i] ) > ret.r + 1e-2 ){
					valid = false;
					break;
				}
			}
			if(valid) break;
		}
		return ret.r;
	}
};


//test
int main(){
	x_rand = XorShift(time(NULL));
	int n;
	cin >> n;
	vector<xy> v(n);
	for(int i=0; i<n; i++){
		double x,y;
		cin >> x >> y;
		v[i] = xy(x,y);
	}
	Smallest_Enclosing_Disk sed(v);
	cout << sed.calc() << endl;
}