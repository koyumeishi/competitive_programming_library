#include <vector>
#include <map>
#include <algorithm>

using namespace std;

class kd_tree{
	//node[i] covers left<= x <= right, bottom <= y <= right
	vector<int> left;
	vector<int> right;
	vector<int> top;
	vector<int> bottom;

	//number of nodes
	int n;

	//root of kd-tree
	int root;
	
	//node infomation < <x,y>, node number >
	vector<pair<pair<int,int>, int> > node;

	//node[i] covers node[j] such that cover[i].first <= j <= cover[i].second
	vector<pair<int,int> > cover;

	//node[i]'s child nodes. if child[i].first == -1 or child[i].second == -1 then node[i]'s left/right child is empty
	vector<pair<int,int> > child;
	
	//node[i] is divided by x axis then true, else false
	vector<bool> divided_by_x;

	//if x == true then this range [begin,end] will be divided by x axis, else by y axis
	int divide(bool x, int begin, int end){
		//out of range
		if(begin > end || begin < 0 || end >= n){
			return -1;
		}
		int l,r,t,b;
		if(x){
			b = node[begin].first.second;
			t = node[end].first.second;
		}else{
			l = node[begin].first.first;
			r = node[end].first.first;
		}
		if(begin != end){
			if(x) sort(node.begin()+begin, node.begin()+end+1, kd_tree::comp_x);
			else sort(node.begin()+begin, node.begin()+end+1, kd_tree::comp_y);
		}
		
		int med = (begin+end+1)/2;
		if(x){
			l = node[begin].first.first;
			r = node[end].first.first;
		}else{
			b = node[begin].first.second;
			t = node[end].first.second;
		}
		left[med] = l;
		right[med] = r;
		top[med] = t;
		bottom[med] = b;

		cover[med] = make_pair(begin, end);
		divided_by_x[med] = x;

		child[med].first = child[med].second = -1;

		if(med-1 >= begin) child[med].first = divide(!x, begin, med-1);
		if(med+1 <= end) child[med].second = divide(!x, med+1, end);

		return med;
	}

	//find nodes in l <= x <= r, b <= y <= t
	void range_search(const int &l, const int &r, const int &b, const int &t, vector<int> &res, int pos){
		if(pos<0) return;
		
		//all nodes are out of the range
		if(left[pos] > r || right[pos] < l || bottom[pos] > t || top[pos] < b) return;
		
		//all nodes are in the range
		if(l<= left[pos] && right[pos] <= r && b <= bottom[pos] && top[pos] <= t){
			for(int i=cover[pos].first; i<=cover[pos].second; i++){
				res.push_back(node[i].second);
			}
			return;
		}

		//some nodes are in the range
		int x = node[pos].first.first;
		int y = node[pos].first.second;

		//node[pos] is contained
		if(l<=x && x<=r && b<=y && y<=t) res.push_back(node[pos].second);

		//about pos's child
		range_search(l,r,b,t, res, child[pos].first);
		range_search(l,r,b,t, res, child[pos].second);
	}
	
public:
	
	static bool comp_x(const pair<pair<int,int>, int> &a, const pair<pair<int,int>, int> &b) {
		return a.first.first < b.first.first;
	}
	
	static bool comp_y(const pair<pair<int,int>, int> &a, const pair<pair<int,int>, int> &b) {
		return a.first.second < b.first.second;
	}

	//node infomation < <x,y>, node number >
	kd_tree(vector< pair<pair<int,int>, int> > v){
		node = v;

		n = node.size();
		
		left.resize(n);
		right.resize(n);
		top.resize(n);
		bottom.resize(n);
		
		cover.resize(n);
		child.resize(n);
		divided_by_x.resize(n);
		
		
		sort(node.begin(), node.end(), kd_tree::comp_y);
		root = divide(true, 0, n-1);
	}


	
	void range_search(const int &l, const int &r, const int &b, const int &t, vector<int> &res){
		range_search(l,r,b,t, res, root);
	}
};
