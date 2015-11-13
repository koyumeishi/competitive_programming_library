
//resize(vector, {v1,v2,v3});

void resize(int& val , vector<int>::iterator itr){val = 0;}
void resize(long long& val , vector<int>::iterator itr){val = 0;}
void resize(double& val , vector<int>::iterator itr){val = 0;}

template<class T>
void resize(vector<T>& vec, vector<int>::iterator itr){
	vec.resize(*itr);
	for(int i=0; i<*itr; i++){
		resize(vec[i], itr+1);
	}
}

template<class T>
void resize(T& vec, vector<int> sz){
	resize(vec, sz.begin());
}