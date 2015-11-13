
long long mod_pow(long long x, long long y, long long MOD){	//x^y mod MOD
	long long ret=1LL;
	while(y>0LL){
		if(y&1LL) ret = (ret * x) % MOD;
		x = (x*x) % MOD;
		y >>= 1LL;
	}
	return ret;
}


long long mod_pow(long long x, long long y){	//x^y
	long long ret=1LL;
	while(y){
		if(y&1LL) ret = (ret * x);
		x = (x*x);
		y >>= 1LL;
	}
	return ret;
}