
long long gcd(long long a, long long b){
	if(b==0) return a;
	return gcd(b, a%b);
}

long long lcm(long long a, long long b){
	if(a<b) swap(a,b);
	if(b==1) return a;
	return a * (b/gcd(a,b));
}


long long extgcd(long long a, long long b, long long &x, long long &y){
	long long d=a;
	if(b!=0){
		d = extgcd(b, a%b, y, x);
		y -= (a/b) * x;
	}else{
		x = 1;
		y = 0;
	}
	return d;
}