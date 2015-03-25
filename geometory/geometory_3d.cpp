
class xyz{
public:
	double x;
	double y;
	double z;

	xyz(){
		
	}
	
	xyz(double xx, double yy, double zz){
		x = xx;
		y = yy;
		z = zz;
	}

	xyz(const xyz &v){
		x = v.x;
		y = v.y;
		y = v.z;
	}

	xyz& operator=(const xyz &v){
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	
	xyz operator+(const xyz &v) const{
		return xyz(this->x+v.x, this->y+v.y, this->z+v.z);
	}
	
	xyz operator-(const xyz &v) const{
		return xyz(this->x-v.x, this->y-v.y, this->z-v.z);
	}
};

xyz cross(const xyz &u, const xyz &v){
	return xyz( u.y*v.z-u.z*v.y, u.z*v.x-u.x*v.z, u.x*v.y-u.y*v.x);
}

double dot(const xyz &u, const xyz &v){
	return u.x*v.x + u.y*v.y + u.z*v.z;
}


//get the distance between the plane Q and the point P
//the plane consists of q[0],q[1] and q[2]
//ax+by+cz+d=0

//the point is p
double dist_plane_point(const vector<xyz> &q, const xyz &p){
	//norm vector
	xyz n = cross(q[1]-q[0], q[2]-q[0]);
	double d = -n.x*q[0].x -n.y*q[0].y -n.z*q[0].z;

	ans += abs( dot(p,n) + d )/sqrt(dot(n,n));
	
}
