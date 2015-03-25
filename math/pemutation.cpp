//using BIT



//n is the size of vector
//vector's elements must be distinct
vector<int> get_Xth_permutation(int n, long long x){

	x--;
	vector<int> ord(n);
	for(int i=1; i<n; i++){
		ord[n-i] = x%i + 1;
		x /= i;
	}
	ord[0] = x + 1;

	BinaryIndexedTree_1_indexed bit(n);
	for(int i=0; i<n; i++){
		bit.add(i+1, 1);
	}
	
	vector<int> ret(n);
	for(int i=0; i<n; i++){
		int pos = bit.lower_bound(ord[i]);
		bit.add(pos, -1);
		ret[i] = pos - 1;
	}

	return ret;
}


//vector v is Xth smallest permutation in the lexicographical order
long long permutation_is_Xth(vector<int> v){
	int n=v.size();
	vector<int> ord(n);
	
	BinaryIndexedTree_1_indexed bit(n);
	for(int i=n-1; i>=0; i--){
		ord[i] = bit.get_sum(v[i]+1);
		bit.add(v[i]+1, 1);
	}

	long long x = ord[0]*(n-1LL);
	for(int i=1; i<n-1; i++){
		x += ord[i];
		x *= (n-1-i);
	}
	x += ord[n-1];

	return x+1;
}

