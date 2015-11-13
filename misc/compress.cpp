template<class T>
vector<T> compress(vector<T> a){
	auto b = a;
	sort(b.begin(), b.end());
	b.erase( unique(b.begin(), b.end()), b.end());
	for(int i=0; i<a.size(); i++){
		a[i] = lower_bound(b.begin(), b.end(), a[i]) - b.begin();
	}
	return a;
}

template<class T>
vector<T> compress(vector<T> a, int& sz){
	auto b = a;
	sort(b.begin(), b.end());
	b.erase( unique(b.begin(), b.end()), b.end());
	for(int i=0; i<a.size(); i++){
		a[i] = lower_bound(b.begin(), b.end(), a[i]) - b.begin();
	}
	sz = b.size();
	return a;
}

