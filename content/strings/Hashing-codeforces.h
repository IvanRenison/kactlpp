/**
 * Author: Simon Lindholm
 * Date: 2015-03-15
 * License: CC0
 * Source: own work
 * Description: Various self-explanatory methods for string hashing.
 * Use on Codeforces, which lacks 64-bit support and where solutions can be hacked.
 * Status: stress-tested
 */
#pragma once

typedef uint64_t ull;
static ll C; // initialized below

// Arithmetic mod two primes and 2^32 simultaneously.
// "typedef uint64_t H;" instead if Thue-Morse does not apply.
template<ll M, class B>
struct A {
	ll x; B b; A(ll x=0) : x(x), b(x) {}
	A(ll x, B b) : x(x), b(b) {}
	A operator+(A o){ll y = x+o.x; return{y - (y>=M)*M, b+o.b};}
	A operator-(A o){ll y = x-o.x; return{y + (y< 0)*M, b-o.b};}
	A operator*(A o) { return {(ll)(1LL*x*o.x % M), b*o.b}; }
	explicit operator ull() { return x ^ (ull) b << 21; }
	bool operator==(A o) const { return (ull)*this == (ull)o; }
	bool operator<(A o) const { return (ull)*this < (ull)o; }
};
typedef A<1000000007, A<1000000009, unsigned>> H;

struct HashInterval {
	vector<H> ha, pw;
	HashInterval(string& str) : ha(SZ(str)+1), pw(ha) {
		pw[0] = 1;
		fore(i,0,SZ(str))
			ha[i+1] = ha[i] * C + str[i],
			pw[i+1] = pw[i] * C;
	}
	H hashInterval(ll a, ll b) { // hash [a, b)
		return ha[b] - ha[a] * pw[b - a];
	}
};

vector<H> getHashes(string& str, ll length) {
	if (SZ(str) < length) return {};
	H h = 0, pw = 1;
	fore(i,0,length)
		h = h * C + str[i], pw = pw * C;
	vector<H> ret = {h};
	fore(i,length,SZ(str)) {
		ret.pb(h = h * C + str[i] - pw * str[i-length]);
	}
	return ret;
}

H hashString(string& s){H h{}; for(char c:s) h=h*C+c;return h;}

#include <sys/time.h>
int main() {
	timeval tp;
	gettimeofday(&tp, 0);
	C = (ll)tp.tv_usec; // (less than modulo)
	assert((ull)(H(1)*2+1-3) == 0);
	// ...
}
