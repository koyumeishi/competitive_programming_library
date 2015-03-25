//convex_hull
vector<xy> convex_hull(vector<xy> &v){
	sort( v.begin(), v.end(), comp_xy );
	int k = 0;	//nums of vertex
	vector<xy> tmp(v.size()*2);
	//conect i from k
	for(int i=0; i<v.size(); i++){
		while(k>1 && cross (tmp[k-1] - tmp[k-2], v[i] - tmp[k-1]) <= 0 ) k--;
		tmp[k] = v[i];
		k++;
	}
	
	for(int i=v.size()-2, t=k; i>=0; i--){
		while(k>t && cross(tmp[k-1] - tmp[k-2], v[i] - tmp[k-1]) <= 0 ) k--;
		tmp[k] = v[i];
		k++;
	}

	tmp.resize(k-1);
	return tmp;
}

pair<int, int> farthest_point_pair(vector<xy> &v /* v MUST BE CONVEX HULL*/){
	int n = v.size();
	
	int p = 0;	//the point which has min y
	int q = 0;	//the point which has max y
	for(int i=0; i<n; i++){
		if( v[p].y > v[i].y ) p = i;
		if( v[q].y < v[i].y ) q = i;
	}

	double max_dist = dist_p_p( v[p], v[q] );
	int ret_a = p;
	int ret_b = q;
	
	int s = p;
	int t = q;
	while(1){
		if( cross( v[(s+1)%n]-v[s], v[(t+1)%n] - v[t] ) < 0 ) s = (s+1)%n;
		else t = (t+1)%n;

		if( dist_p_p( v[s], v[t] ) > max_dist ){
			ret_a = s;
			ret_b = t;
			max_dist = dist_p_p( v[s], v[t] );
		}
		
		if( s==p && t==q ) break;
	}
	return make_pair(ret_a, ret_b);
}

