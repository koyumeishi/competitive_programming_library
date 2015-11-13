
template<class T>
ostream& operator << (ostream& os, vector<T> vec){
	for(int i=0; i<vec.size(); i++){
		os << vec[i] << " ";
	}
	return os;
}