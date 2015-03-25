#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <map>
#include <climits>
#include <cmath>
#include <cfloat>
#include <cstdio>
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

//zero -> p1
int ccw(xy p1, xy p2, xy p3){
	p2 -= p1;
	p3 -= p1;
	double c = cross(p2,p3);
	if( c > EPS /* c > 0 */) return +1;			//counter-clockwise
	if( c < -EPS /* c < 0 */) return -1;		//clock-wise
	if( dot(p2,p3) < -EPS) return +2;			//on segment : p3-p1-p2
	if( dot(p3,p3) < dot(p2,p2) +EPS ) return -2;	//on segment : p1-p3-p2
	return 0;
}

//segment p1-p2, p3-p4
bool inter_ss(xy p1, xy p2, xy p3, xy p4){
	return ( (ccw(p1,p2, p3) * ccw(p1,p2, p4) <= 0) && (ccw(p3,p4, p1) * ccw(p3,p4, p2) <= 0 ) ) ;
}
