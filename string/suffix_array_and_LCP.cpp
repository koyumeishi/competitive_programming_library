
class SuffixArray{
private:
	vector<int> rank;

	string S;
	int n;

	void init(string &SS){
		S = SS;
		n = S.size();

		sa.resize(n+1);
		lcp.resize(n+1);
		
		rank = vector<int>(n+1);
	}

public:
	vector<int> sa;
	vector<int> lcp;

	SuffixArray(){

	}
	
	SuffixArray(string &SS){
		init(SS);

		constract_sa();
		constract_lcp();

	}


	//Manber & Myers
	//O(N (logN)^2)
	void constract_sa(){
		
		
		//first character
		for(int i=0; i<=n; i++){
			sa[i] = i;
			rank[i] = (i<n) ? S[i] : -1;
		}

		vector<int> tmp(n+1);
		for(int k=1; k<=n; k *= 2){

			auto cmp_func = [&](int x, int y) -> bool{
				if(rank[x] != rank[y]) return rank[x] < rank[y];
				int l = (x+k <= n) ? rank[x+k] : -1;
				int r = (y+k <= n) ? rank[y+k] : -1;
				return l < r;
			};

			sort(sa.begin(), sa.end(), cmp_func);

			tmp[sa[0]] = 0;
			for(int i=1; i<=n; i++){
				tmp[sa[i]] = tmp[sa[i-1]] + (cmp_func(sa[i-1],sa[i])==true?1:0);
			}
			for(int i=0; i<=n; i++){
				rank[i] = tmp[i];
			}
		}
	}
	
	void constract_lcp(){
		for(int i=0; i<=n; i++) rank[sa[i]] = i;
		int h=0;
		lcp[0] = 0;
		for(int i=0; i<n; i++){
			int j=sa[rank[i]-1];

			if(h>0) h--;
			for(; j+h<n && i+h<n; h++){
				if(S[j+h] != S[i+h]) break;
			}

			lcp[rank[i] - 1] = h;
		}
	}

	void dbg_print(){
		for(int i=0; i<=n; i++){
			string p(S.begin() + sa[i], S.end());
			cerr << p << " " << lcp[i] << endl;
		}
	}
};



//palindromes
int get_num_of_palindromes(string s){

		int n = s.size();

		string r(s.rbegin(), s.rend());

		string S = s + "$" + r;
		SuffixArray SA(S);
		//SA.dbg_print();


		SegmentTreeMIN seg(S.size()+1);
		for(int i=0; i<=S.size(); i++){
			//seg.debug();
			int at = SA.get_rank(i);
			int val = SA.lcp[at];
			seg.update(at, val);
		}
		//seg.debug();

		long long ans = 0;
		{
			vector<long long> sum(n+10, 0);
			for(int i=0; i<n; i++){
				int j = n*2-i;
				int l = min( SA.get_rank(i), SA.get_rank(j) );
				int r = max( SA.get_rank(i), SA.get_rank(j) );
				int len = seg.RMQ( l, r);
				int cnt = 2*len-1;
				sum[1] += 1;
				sum[cnt+2] -= 1;
			}
			long long x = 0;
			for(int i=1; i<=n; i+=2){
				x += sum[i];
				ans += x;
				ans %= MOD;
			}
		}
		{
			vector<long long> sum(n+10, 0);
			for(int i=1; i<n; i++){
				int j = n*2-i+1;
				int l = min( SA.get_rank(i), SA.get_rank(j) );
				int r = max( SA.get_rank(i), SA.get_rank(j) );
				int len = seg.RMQ( l, r);
				int cnt = 2*len;
				sum[2] += 1;
				sum[cnt+2] -= 1;
			}
			long long x = 0;
			for(int i=2; i<=n; i+=2){
				x += sum[i];
				ans += x;
				ans %= MOD;
			}
		}

		return ans%MOD;

}