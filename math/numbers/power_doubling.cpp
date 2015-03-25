
long long mypow(long long x, long long y, long long MOD){
	long long ret=1LL;
	while(y>0LL){
		if(y&1LL) ret = (ret * x) % MOD;
		x = (x*x) % MOD;
		y >>= 1LL;
	}
	return ret;
}