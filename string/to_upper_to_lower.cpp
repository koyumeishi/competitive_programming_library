#include <string>

using namespace std;

string to_uppercase(string s){
	for(auto &c: s){
		c &= ~32;
	}
	return s;
}

string to_lowercase(string s){
	for(auto &c: s){
		c |= 32;
	}
	return s;
}

string flip_uppercase_lowercase(string s){
	for(auto &c: s){
		c ^= 32;
	}
	return s;
}


#include <iostream>
int main(){
	string s;
	cin >> s;
	cout << to_uppercase(s) << endl;
	cout << to_lowercase(s) << endl;
	cout << flip_uppercase_lowercase(s) << endl;
	return 0;
}