//sqrt decomposition

#define SZ

{
	int bucket_size = n/SZ + 1;
	vector<vector<int>> bucket(bucket_size, vector<int>(SZ));
	vector<int> rep(bucket_size);

}

{	//initialize
	for(int i=0; i<n; i++){
		bucket[i/SZ][i%SZ] = v[i];
		rep[i/SZ] += v[i];
	}
}

{
	//[l,r];

	int ld = l/SZ, rd = r/SZ;
	int lm = l%SZ, rm = r%SZ;
	if(ld == rd){
		//do something to bucket[ld]
		for(int k=lm; k<=rm; k++){
			bucket[ld][k] = do something;
		}

	}else{
		//do something to bucket[ld]
		for(int k=lm; k<SZ; k++){
			bucket[ld][k] = do something;
		}

		//do something to bucket[rd]

		for(int k=0; k<=rm; k++){
			bucket[rd][k] = do something;
		}

		ld++;
		for(; ld<rd; ld++){
			//do something
			rep[ld];
		}
	}
}