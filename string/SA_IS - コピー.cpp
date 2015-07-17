#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <map>
#include <set>
#include <cmath>
#include <cstdio>
#include <cstdlib>

using namespace std;

//Suffix Array SA-IS algorithm
//O(N), using too many memory
//verified 2015/03/24 03:30 at
// https://www.hackerrank.com/contests/w7/challenges/string-function-calculation
// http://codeforces.com/contest/529/problem/A

class SA_IS{
	//compress alphabets
	//return vector<count of alphabet>
	vector<int> compress(vector<int>& v){
		set<int> alph(v.begin(), v.end());
		vector<int> dic(alph.begin(), alph.end());
		vector<int> bucket_size(dic.size(),0);
		for(int i=0; i<v.size(); i++){
			v[i] = lower_bound(dic.begin(), dic.end(), v[i]) - dic.begin();
			bucket_size[v[i]]++;
		}
		return bucket_size;
	}
public:
	static const int L = 0;
	static const int S = 1;
	
	string my_s;
	vector<int> sa;
	vector<int> lcp;
	
	struct substring{
		int pos;
		int size;
		vector<int>::iterator str;
		vector<int>::iterator ls;
		substring(){
		}
		substring(const int pos, const int size, const vector<int>::iterator& str, const vector<int>::iterator& ls){
			this->pos = pos;
			this->size = size;
			this->str = str;
			this->ls = ls;
		}
		bool operator<(const substring& x) const {
			for(int i=0; i<min(this->size, x.size); i++){
				if(this->str[i] != x.str[i]){
					return this->str[i] < x.str[i];
				}else if(this->ls[i] != x.ls[i]){
					return this->ls[i] == SA_IS::L;
				}
			}
			return this->size < x.size;
		}
		bool operator==(const substring& x) const {
			for(int i=0; i<min(this->size, x.size); i++){
				if(this->str[i] != x.str[i]){
					return false;
				}else if(this->ls[i] != x.ls[i]){
					return false;
				}
			}
			return this->size == x.size;
		}
		substring operator=(const substring& x){
			this->pos = x.pos;
			this->size = x.size;
			this->str = x.str;
			this->ls = x.ls;
			return *this;
		}
	};
	
	vector<int> rec(vector<int>& v){
		v.push_back(-1);
		vector<int> bucket_size = compress(v);

		//bucket
		vector<int> ret(v.size(), -1);
		vector<vector<int>::iterator> bucket(bucket_size.size());
		vector<bool> fixed(v.size(), false);
		vector<vector<bool>::iterator> bucket_fixed(bucket_size.size());
		vector<int> bucket_front(bucket_size.size());
		vector<int> bucket_back(bucket_size.size());
		vector<bool> used(v.size(), false);
		
		//initialize
		{
			int sum = 0;
			for(int i=0; i<bucket_size.size(); i++){
				bucket[i] = ret.begin() + sum;
				bucket_fixed[i] = fixed.begin() +sum;
				bucket_front[i] = 0;
				bucket_back[i]  = bucket_size[i] - 1;
				sum += bucket_size[i];
			}
		}
		
		vector<int> LS(v.size());
		LS.back() = S;
		for(int i=v.size()-2; i>=0; i--){
			if(v[i] < v[i+1])		LS[i] = S;
			else if (v[i] > v[i+1]) LS[i] = L;
			else 					LS[i] = LS[i+1];
		}

		
		vector<int> LMS;
		for(int i=1; i<v.size(); i++){
			if(LS[i] != S) continue;
			if(LS[i-1] == L) LMS.push_back(i);
		}
		LMS.push_back(v.size()-1);
		
		
		vector< pair<substring, int> > LMS_substring;
		for(int i=0; i<LMS.size()-1; i++){
			substring sub(LMS[i], LMS[i+1]-LMS[i]+1, v.begin()+LMS[i], LS.begin()+LMS[i]);
			LMS_substring.push_back({sub, LMS_substring.size()});
		}

		if(LMS_substring.size() > 0){
			vector< pair<substring, int>> LMS_substring_old = LMS_substring;

			sort(LMS_substring.begin(), LMS_substring.end());

			vector<int> LMS_order(LMS_substring.size());

			int cnt = 0;
			LMS_order[ LMS_substring[0].second ] = cnt;
			cnt++;
			for(int i=1; i<LMS_substring.size(); i++){
				if(LMS_substring[i].first == LMS_substring[i-1].first){
					LMS_order[ LMS_substring[i].second ] = cnt-1;
				}else{
					LMS_order[ LMS_substring[i].second ] = cnt;
					cnt++;
				}
			}

			if(cnt != LMS_substring.size()){
				vector<int> new_order = rec(LMS_order);
				LMS_order = vector<int>(new_order.begin()+1, new_order.end());
				for(int i=0; i<LMS_substring.size(); i++){
					LMS_substring[i].first = LMS_substring_old[LMS_order[i]].first;
				}
			}

			for(int i=LMS_substring.size()-1; i>=0; i--){
				int c = v[LMS_substring[i].first.pos];
				bucket[c][bucket_back[c]] = LMS_substring[i].first.pos;
				bucket_back[c]--;
			}
		}


		for(int i=0; i<bucket_size.size(); i++){
			for(int j=0; j<bucket_size[i]; j++){
				if( bucket[i][j] - 1 < 0 ) continue;
				if( LS[ bucket[i][j]-1 ] == L ){
					if(used[bucket[i][j]-1]) continue;
					used[ bucket[i][j]-1 ] = true;
					
					int c = v[ bucket[i][j]-1 ];
					bucket[c][ bucket_front[c] ] = bucket[i][j]-1;
					bucket_fixed[c][ bucket_front[c] ] = true;
					bucket_front[c]++;
				}
			}
			bucket_back[i] = bucket_size[i] - 1;
		}
		for(int i=0; i<v.size(); i++){
			if(fixed[i] == false){
				ret[i] = -1;
			}
		}
		
		for(int i=bucket_size.size()-1; i>=0; i--){
			for(int j=bucket_size[i]-1; j>=0; j--){
				if( bucket[i][j] - 1 < 0 ) continue;
				if( LS[ bucket[i][j]-1 ] == S ){
					if(used[bucket[i][j]-1]) continue;
					used[ bucket[i][j]-1 ] = true;
					
					int c = v[ bucket[i][j]-1 ];
					bucket[c][ bucket_back[c] ] = bucket[i][j]-1;
					bucket_fixed[c][ bucket_back[c] ] = true;
					bucket_back[c]--;
				}
			}
		}
		
		ret[0] = ret.size()-1;
	
		return ret;
	}
	
	vector<int> rec(string &s){
		vector<int> v(s.begin(), s.end());
		return rec(v);
	}
	
	void constract_lcp(){
		int n = my_s.size();
		lcp.resize(n+1);
		vector<int> rank(n+1);
		for(int i=0; i<=n; i++) rank[sa[i]] = i;
		int h=0;
		lcp[0] = 0;
		for(int i=0; i<n; i++){
			int j=sa[rank[i]-1];
			if(h>0) h--;
			for(; j+h<n && i+h<n; h++){
				if(my_s[j+h] != my_s[i+h]) break;
			}
			lcp[rank[i] - 1] = h;
		}
	}

	SA_IS(string s){
		my_s = s;
		sa = rec(s);
		constract_lcp();
	}
	
	void dbg_print(){
		for(int i=0; i<=my_s.size(); i++){
			string p(my_s.begin() + sa[i], my_s.end());
			cerr << p << " " << sa[i] << /*" " << lcp[i] <<*/ endl;
		}
	}
};

#include <ctime>

int main(){
	string s;
	cin >> s;
	auto start = clock();
	SA_IS sa(s);
	cout << (clock() - start) << endl;
	//sa.dbg_print();
	
	return 0;
}