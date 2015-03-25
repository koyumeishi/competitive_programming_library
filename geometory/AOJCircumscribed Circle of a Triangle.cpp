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


class Smallest_Enclosing_Disk{
	const static int INF = (1<<30);
	priority_queue< pair<int,int> > pq;
	vector<xy> v;

	typedef struct{
		double r;
		xy O;
	}disk;
	
	pair<vector<int>, disk> make_disk_from_2_points(vector<int> R){
		disk ret;
		ret.O.x = (v[R[0]].x + v[R[1]].x) * 0.5;
		ret.O.y = (v[R[0]].y + v[R[1]].y) * 0.5;
		ret.r = dist_p_p(v[R[0]], v[R[1]]) * 0.5;
		if(ret.r == 0) R.erase(R.begin()+1);
		return make_pair(R, ret);
	}

	pair<vector<int>, disk> make_disk_from_3_points(vector<int> R){
		disk ret;
		
		if(dot(v[R[1]] - v[R[0]], v[R[2]] - v[R[0]]) <= 0){
			pair<vector<int>, disk> d = make_disk_from_2_points( vector<int>{R[1], R[2]});
			R = d.first;
			ret = d.second;
			
		}else if(dot(v[R[0]] - v[R[1]], v[R[2]] - v[R[1]]) <= 0){
			pair<vector<int>, disk> d = make_disk_from_2_points( vector<int>{R[0], R[2]});
			R = d.first;
			ret = d.second;
			
		}else if(dot(v[R[0]] - v[R[2]], v[R[1]] - v[R[2]]) <= 0){
			pair<vector<int>, disk> d = make_disk_from_2_points( vector<int>{R[0], R[1]});
			R = d.first;
			ret = d.second;
			
		}else{
			double A = dot(v[R[0]], v[R[0]]);
			double B = dot(v[R[1]], v[R[1]]);
			double C = dot(v[R[2]], v[R[2]]);

			//r(x,y) = { A(B+C-A)r_a + B(C+A-B)r_b + C(A+B-C)r_c } / 16S^2
			//16S^2 = A(B+C-A) + B(C+A-B) + C(A+B-C)

			double S_ = A*(B+C-A) + B*(C+A-B) + C*(A+B-C);
			ret.O.x = (A*(B+C-A) * v[R[0]].x + B*(C+A-B) * v[R[1]].x + C*(A+B-C) * v[R[2]].x) / S_;
			ret.O.y = (A*(B+C-A) * v[R[0]].y + B*(C+A-B) * v[R[1]].y + C*(A+B-C) * v[R[2]].y) / S_;
			ret.r = dist_p_p( v[R[0]], ret.O ) * 0.5;
		}

		return make_pair( R, ret);
	}
	
	//R := index of points which are on disk border
	//S := smallest enclosing disk
	//P := new added point
	pair<vector<int>, disk> D(vector<int> R, disk S, int P){
		//P is inside of S
		if( (dist_p_p(v[P], S.O) <= S.r) && (R.size() > 0) ){
			return make_pair(R, S);
		}
		disk ret;
		if(R.size() == 0){
			R.push_back(P);
			ret.O = v[P];
			ret.r = 0.0;
		}else if(R.size() == 1){
			R.push_back(P);
			
			pair<vector<int>, disk> d = make_disk_from_2_points(R);
			R = d.first;
			ret = d.second;
			
		}else if(R.size() == 2){
			R.push_back(P);
			pair<vector<int>, disk> d = make_disk_from_3_points(R);
			R = d.first;
			ret = d.second;
		}else if(R.size() == 3){
			vector<int> tmp_R;
			disk tmp_S;
			for(int i=0; i<3; i++){
				pair<vector<int>, disk> d = make_disk_from_3_points(vector<int>{R[i%3],R[(i+1)%3], P});

				//disk can't enclose every point
				if(dist_p_p(v[R[(i+2)%3]], d.second.O) > d.second.r){
					d.second.r = INF;
				}
				if(i==0 || d.second.r < tmp_S.r){
					tmp_R = d.first;
					tmp_S = d.second;
				}
			}
			R = tmp_R;
			ret = tmp_S;
		}
		return make_pair(R,ret);
	}

public:
	//r, xy
	Smallest_Enclosing_Disk(vector<xy> &v){
		this->v = v;
		//random key, index
		for(int i=0; i<v.size(); i++){
			pq.push( make_pair(rand(), i) );
		}
	}

	double calc(){
		vector<int> R;
		disk ret;
		while(!pq.empty()){
			pair<vector<int>, disk> d = D(R, ret, pq.top().second);
			pq.pop();
			R = d.first;
			ret = d.second;
		}
		return ret.r;
	}
};



//test
int main(){
	srand(time(NULL));
	int n;
	cin >> n;
	for(int i=0; i<n; i++){
		vector<xy> v(3);
		for(int j=0; j<3; j++){
			double x,y;
			cin >> x >> y;
			v[j] = xy(x,y);
		}
		xy O;
		double r;
		// rA = v[0]
		// rB = v[1]
		// rC = v[2]
		xy a = v[2] - v[1];
		xy b = v[2] - v[0];
		xy c = v[1] - v[0];
		double A = dot(a, a);
		double B = dot(b, b);
		double C = dot(c, c);

		//r(x,y) = { A(B+C-A)r_a + B(C+A-B)r_b + C(A+B-C)r_c } / 16S^2
		//16S^2 = A(B+C-A) + B(C+A-B) + C(A+B-C)

		double S_ = A*(B+C-A) + B*(C+A-B) + C*(A+B-C);
		O.x = (A*(B+C-A) * v[0].x + B*(C+A-B) * v[1].x + C*(A+B-C) * v[2].x) / S_;
		O.y = (A*(B+C-A) * v[0].y + B*(C+A-B) * v[1].y + C*(A+B-C) * v[2].y) / S_;
		r = dist_p_p( v[0], O );

		printf("%.3f %.3f %.3f\n", O.x, O.y, r);
	}
}