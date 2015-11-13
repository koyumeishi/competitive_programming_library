//O (sqrt a)
long long CarmichaelFunction(long long a){
	long long ret = 1;
	for(long long i=2; i*i<=a; i++){
		if(a%i != 0) continue;
		int cnt = 0;
		long long x = 1;
		while(a%i == 0){
			a /= i;
			x *= i;
			cnt++;
		}

		if(i==2){
			if(cnt <= 2) ret *= i;
			else ret *= 1<<(cnt-2);
		}else{
			ret = lcm(ret, x/i * (i-1));
		}
	}
	if(a>1){
		ret = lcm(ret, a-1);
	}
	return ret;
}



pair<int,int> hoge(long long x, long long m){
	int u=0, v=0;
	long long g = gcd(x,m);
	while(g != 1){
		m /= g;
		u++;
		g = gcd(x,m);
	}
	long long l = CarmichaelFunction(m);
	v = l;
	return {u,v};
}