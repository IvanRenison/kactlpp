// Problem: https://www.spoj.com/problems/HORRIBLE/
// Status: accepted
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) begin(x), end(x)
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

struct STree { // example: range sum with range addition
	typedef ll T; typedef ll L; // T: data type, L: lazy type
	constexpr static T tneut = 0; constexpr static L lneut = 0;
	// neutrals
	T f(T a, T b) { return a + b; } // operation
	T apply(T v, L l, ll s, ll e) { return v + l * (e - s); }
	// new st according to lazy
	L comb(L a, L b) { return a + b; }
	// cumulative effect of lazy

	vector<T> st;
	vector<L> lazy;
	ll n;
	STree(ll n) : st(4*n, tneut), lazy(4*n, lneut), n(n) {}
	void init(ll k, ll s, ll e, const vector<T> &a) {
		lazy[k] = lneut;
		if (s + 1 == e) { st[k] = a[s]; return; }
		ll m = (s + e) / 2;
		init(2*k, s, m, a), init(2*k+1, m, e, a);
		st[k] = f(st[2*k], st[2*k+1]);
	}
	void push(ll k, ll s, ll e) {
		if (lazy[k] == lneut) return; // if neutral, nothing to do
		st[k] = apply(st[k], lazy[k], s, e);
		if (s + 1 < e) { // propagate to children
			lazy[2*k] = comb(lazy[2*k], lazy[k]);
			lazy[2*k+1] = comb(lazy[2*k+1], lazy[k]);
		}
		lazy[k] = lneut; // clear node lazy
	}
	void upd(ll k, ll s, ll e, ll a, ll b, L v) {
		push(k, s, e);
		if (s >= b || e <= a) return;
		if (s >= a && e <= b) {
			lazy[k] = comb(lazy[k], v); // accumulate lazy
			push(k, s, e);
			return;
		}
		ll m = (s + e) / 2;
		upd(2*k, s, m, a, b, v), upd(2*k+1, m, e, a, b, v);
		st[k] = f(st[2*k], st[2*k+1]);
	}
	T query(ll k, ll s, ll e, ll a, ll b) {
		if (s >= b || e <= a) return tneut;
		push(k, s, e);
		if (s >= a && e <= b) return st[k];
		ll m = (s + e) / 2;
		return f(query(2*k, s, m, a, b),query(2*k+1, m, e, a, b));
	}
	void init(const vector<T> &a) { init(1, 0, n, a); }
	void upd(ll a, ll b, L v) { upd(1, 0, n, a, b, v); }
	T query(ll a, ll b) { return query(1, 0, n, a, b); }
};

void solveCase() {
	ll N, C;
	cin >> N >> C;

	STree st(N);

	fore(_, 0, C) {
		ll ty, p, q;
		cin >> ty >> p >> q;
		p--;

		if (ty == 0) { // Update
			ll v;
			cin >> v;
			st.upd(p, q, v);
		} else { // Query
			ll ans = st.query(p, q);
			cout << ans << '\n';
		}
	}
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll T;
	cin >> T;

	fore(_, 0, T) {
		solveCase();
	}
}