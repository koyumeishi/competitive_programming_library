
//palindrome
class Manacher{
	string s;
	int n;
	vector<int> radius;
	
public:
	Manacher(string &s_){
		n = s_.size();
		s = string(2*n+1, '#');
		for(int i=0; i<n; i++){
			s[2*i + 1] = s_[i];
		}
		n = s.size();

		radius = vector<int>(n, -1);
		for(int i=0, j=0; i<n; ){
			while(1){
				if(i-j < 0 || i+j >= n) break;

				if(s[i-j] == s[i+j]) j++;
				else break;
			}
			radius[i] = j;

			int k = 1;
			while(1){
				if(i-k < 0 || i+k >= n) break;

				if(radius[i-k] < radius[i] - k){
					radius[i+k] = radius[i-k];
					k++;
				}else break;
			}

			i += k;
			j = max(j-k, 0);
		}
	}

	int get_longest_palindrome(){
		return (int)*max_element(radius.begin(), radius.end());
	}

	int count_palindromes(){
		int MOD = 1000000007;
		vector<long long> sum(n+10, 0);
		for(int i=0; i<n; i++){
			int len;
			if(i%2==0){
				len = (radius[i]/2) * 2;
			}else{
				len = ((radius[i]-1)/2) * 2 + 1;
			}
			sum[(len%2==0)?2:1] += 1;
			sum[len+2] -= 1;
		}

		long long ret = 0;
		vector<long long> tmp(2, 0);
		for(int i=1; i<=n; i++){
			tmp[i&1] += sum[i];
			tmp[i&1] %= MOD;
			ret += tmp[i&1] * i;
			ret %= MOD;
		}
		return ret;
	}
};
