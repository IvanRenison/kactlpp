/**
 * Author: Iván Renison
 * Date: 2024-04-02
 * License: CC0
 * Source: notebook el vasito
 * Description: Max segment tree in which each update creates a new version of the tree
 * 	and you can query and update on any version of the tree. \texttt{init} and
 * 	\texttt{upd} return the new version number.
 * Can be changed by modifying T, f and unit.
 * Time: O(\log N)
 * Usage: Tree rmq(n);
 *  ver = rmq.init(xs);
 *  new_ver = rmq.upd(ver, i, x);
 *  rmq.query(ver, l, u);
 * Status: Tested with codeforces 893F and stress-tested a bit
 */
#pragma once

struct Tree {
	typedef ll T;
	static constexpr T neut = LONG_LONG_MIN;
	T f(T a, T b) { return max(a, b); } // (any associative fn)

	vector<T> st;
	vi L, R;
	ll n, rt;
	Tree(ll n) : st(1, neut), L(1), R(1), n(n), rt(0) {}
	ll new_node(T v, ll l, ll r) {
		st.pb(v), L.pb(l), R.pb(r);
		return SZ(st) - 1;
	}
	// not necessary in most cases
	ll init(ll s, ll e, vector<T>& a) {
		if (s + 1 == e) return new_node(a[s], 0, 0);
		ll m = (s + e) / 2, l = init(s, m, a), r = init(m, e, a);
		return new_node(f(st[l], st[r]), l, r);
	}
	ll upd(ll k, ll s, ll e, ll p, T v) {
		ll ks = new_node(st[k], L[k], R[k]);
		if (s + 1 == e) {
			st[ks] = v;
			return ks;
		}
		ll m = (s + e) / 2;
		if (p < m) L[ks] = upd(L[ks], s, m, p, v);
		else R[ks] = upd(R[ks], m, e, p, v);
		st[ks] = f(st[L[ks]], st[R[ks]]);
		return ks;
	}
	T query(ll k, ll s, ll e, ll a, ll b) {
		if (e <= a || b <= s) return neut;
		if (a <= s && e <= b) return st[k];
		ll m = (s + e) / 2;
		return f(query(L[k], s, m, a, b), query(R[k], m, e, a, b));
	}
	ll init(vector<T>& a) { return init(0, n, a); }
	ll upd(ll ver, ll p, T v) {return rt = upd(ver, 0, n, p, v);}
	// upd on last root
	ll upd(ll p, T v) { return upd(rt, p, v); }
	T query(ll ver, ll a, ll b) {return query(ver, 0, n, a, b);}
};
