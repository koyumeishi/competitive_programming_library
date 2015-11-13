//bitset manipulation

#include <bitset>
#define SZ 100000
typedef bitset<SZ> bits;

//00011111
bitset get_mask(int len){
	bits mask;
	mask = (~mask)>>(SZ-len);
}

//00001100
//[l,r)
bitset make_range_mask(int l, int r, bitset& mask, int len){
	return ((mask>>(len-r+l))<<l);
}

