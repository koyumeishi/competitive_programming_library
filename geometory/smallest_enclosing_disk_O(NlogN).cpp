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

#define EPS 1e-4

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


//random pick algorithm
//O(NlogN)?
class Smallest_Enclosing_Disk{
	vector<xy> v;

	typedef struct{
		double r;
		xy O;
	}disk;
	
	disk make_disk_from_points(int P){
		disk ret;
		ret.r = 0.0;
		ret.O = v[ P ];
		return ret;
	}
	
	disk make_disk_from_points(int P, int Q){
		disk ret;
		if(v[P].x == v[Q].x && v[P].y == v[Q].y){
			return make_disk_from_points(P);
		}
		ret.O.x = (v[P].x + v[Q].x) * 0.5;
		ret.O.y = (v[P].y + v[Q].y) * 0.5;
		ret.r = dist_p_p(v[P], v[Q]) * 0.5;
		return ret;
	}

	disk make_disk_from_points(int P, int Q, int R){
		disk ret;
		
		if(dot(v[Q] - v[P], v[R] - v[P]) <= 0 + EPS){
			ret = make_disk_from_points(Q, R);
			
		}else if(dot(v[P] - v[Q], v[R] - v[Q]) <= 0 + EPS){
			ret = make_disk_from_points(P, R);
			
		}else if(dot(v[P] - v[R], v[Q] - v[R]) <= 0 + EPS){
			ret = make_disk_from_points(P, Q);
			
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

		return ret;
	}

public:
	int n;
	//r, xy
	Smallest_Enclosing_Disk(vector<xy> &v){
		this->v = v;
		n = v.size();
	}

	//return r;
	double calc(){
		disk ret;
		if(n == 1){
			ret = make_disk_from_points(0);
		}else if(n == 2){
			ret = make_disk_from_points(0,1);
		}else{
			vector<int> B(n);
			for(int i=0; i<n; i++) B[i] = i;
			
			while(1){
				
				vector<int> R(3);
				R[0] = x_rand.xor128()%(unsigned int)B.size();
				R[1] = x_rand.xor128()%(unsigned int)B.size();
				R[2] = x_rand.xor128()%(unsigned int)B.size();
				while(R[0] == R[1]){
					R[1] = x_rand.xor128()%(unsigned int)B.size();
				}
				while(R[0] == R[2] || R[1] == R[2]){
					R[2] = x_rand.xor128()%(unsigned int)B.size();
				}

				ret = make_disk_from_points(R[0], R[1], R[2]);
				
				B = R;
				
				bool update = false;
				for(int i=0; i<n; i++){
					if( dist_p_p(v[i], ret.O) > ret.r + EPS){
						B.push_back(i);
						update = true;
					}
				}
				
				if(!update) break;
			}
		}
		return ret.r;
	}
};


//test
int main(){
	srand(time(NULL));
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