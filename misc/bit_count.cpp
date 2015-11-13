int bit_count(long long x){
	long long ret = 0;
	while(x){
		ret++;
		x -= x&-x;
	}
	return ret;
}