// X = a*Y + b
// X is big integer > 2^64-1
string rem(string X, long long Y){
	long long b = 0;

	for(int i=0; i<X.size(); i++){
		b *= 10;
		b += X[i] - '0';
		b %= Y;
	}
	return b;
}

// X = a*Y + b
pair<string, long long> divide(string X, long long Y){
	string a = "";
	long long b = 0;

	for(int i=0; i<X.size(); i++){
		b *= 10;
		b += X[i] - '0';
		if(a.size() == 0){
			if(b/Y > 0) a.push_back(b/Y + '0');
		}else{
			a.push_back(b/Y + '0');
		}
		
		b %= Y;
	}
	return {a,b};
}

// base-10 to base-base
string string_to_binary(string X, long long base){
	string ret = "";
	while(true){
		auto tmp = divide(X, base);
		X = tmp.first;
		ret.push_back(tmp.second + '0');
		
		//cerr << X.size() << " " << X[0] << endl;

		if(X.size() == 0){
			break;
		}
	}
	reverse(ret.begin(), ret.end());
	return ret;
}

//val(base-X) -> val(base-10);
long long base_X_to_decimal(string val, int X){
	long long ret = 0;
	long long k = 1;
	for(int i=val.size()-1; i>=0; i--){
		int tmp;
		if('0'<= val[i] && val[i] <= '9'){
			tmp = val[i] - '0';
		}else if('A' <= val[i] && val[i] <= 'Z'){
			tmp = val[i] - 'A' +10;
		}else if('a' <= val[i] && val[i] <= 'z'){
			tmp = val[i] - 'a' +10;
		}else{
			tmp = -1;
		}

		assert(tmp>=0);

		if(tmp >= X) return -1;
		ret += tmp * k;
		k *= X;
	}
	return ret;
}
