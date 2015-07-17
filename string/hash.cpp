#include <string>
#include <unordered_map>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>

template<class T>
size_t get_hash(const T& container, const int from, const int size, size_t seed){
	return std::_Hash_impl::hash(container.data() + from, size, seed);
}

using namespace std;
int main(){
	srand((unsigned)time(NULL));
	size_t seed = rand();
	vector<int> s = {0,1,2,3};
	auto myhash_s = get_hash<vector<int>>(s, 1, 3, seed);

	vector<int> t = {1,2,3};
	auto myhash_t = get_hash<vector<int>>(t, 0, 3, seed);

	cout << myhash_s << endl;
	cout << myhash_t << endl;
	return 0;
}