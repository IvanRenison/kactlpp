/**
 * Author: Iván Renison
 * Date: 2024-09-19
 * License: CC0
 * Source: notebook el vasito
 * Description: Some operations on polynomials made fast with NTT.
 * The may also work with doubles and FFT, but it's numerically unstable.
 * \texttt{inv}, \texttt{log} ans \texttt{exp} return truncated infinite series.
 * \texttt{Poly} elements should not have trailing zeros. The zero polynomial is \texttt{\{\}}.
 * Status: division stress-tested
 */
#pragma once

#include "NumberTheoreticTransform.h"
#include "../number-theory/FastInverse.h"

typedef vi Poly;

Poly add(const Poly& p, const Poly& q) { // O(n)
	Poly res(max(SZ(p), SZ(q)));
	fore(i,0,SZ(p)) res[i] += p[i];
	fore(i,0,SZ(q)) res[i] += q[i];
	for (ll& x : res) x %= mod;
	while (!res.empty() && !res.back()) res.pop_back();
	return res;
}
Poly derivate(const Poly& p) { // O(n)
	Poly res(max(0ll, SZ(p)-1));
	fore(i, 1, SZ(p)) res[i-1] = (i * p[i]) % mod;
	return res;
}
Poly integrate(const Poly& p) { // O(n)
	Poly ans(SZ(p) + 1);
	fore(i, 0, SZ(p)) ans[i+1] = (p[i] * inv(i+1)) % mod;
	return ans;
}

Poly takeMod(Poly p, ll n) { // O(n)
	p.resize(min(SZ(p), n));   // p % (x^n)
	while(!p.empty() && !p.back()) p.pop_back();
	return p;
}

Poly inv(const Poly& p, ll d) { // O(n log(n))
	Poly res = {inv(p[0])};       // first d terms of 1/p
	ll sz = 1;
	while (sz < d){
		sz *= 2;
		Poly pre(p.begin(), p.begin() + min(SZ(p), sz));
		Poly cur = conv(res, pre);
		fore(i, 0, SZ(cur)) if (cur[i]) cur[i] = mod - cur[i];
		cur[0] = cur[0] + 2;
		res = conv(res, cur);
		res = takeMod(res, sz);
	}
	res.resize(d);
	return res;
}
Poly log(const Poly& p, ll d){ // O(n log(n))
	Poly cur = takeMod(p, d);    // first d terms of log(p)
	Poly a = inv(cur, d), b = derivate(cur);
	Poly res = conv(a,b);
	res = takeMod(res, d-1);
	res = integrate(res);
	res.resize(d);
	return res;
}
Poly exp(const Poly& p, ll d) { // O(n log(n)^2)
	Poly res = {1};               // first d terms of exp(p)
	ll sz = 1;
	while (sz < d) {
		sz *= 2;
		Poly lg = log(res, sz), cur(sz);
		fore(i, 0, sz) cur[i] = (mod + (i<SZ(p) ? p[i] : 0)
			- (i<SZ(lg) ? lg[i] : 0)) % mod;
		cur[0] = (cur[0] + 1) % mod;
		res = conv(res, cur);
		res = takeMod(res, sz);
	}
	res.resize(d);
	return res;
}

pair<Poly,Poly> div(const Poly& a, const Poly& b){
	ll m = SZ(a), n = SZ(b);   // O(n log(n)), returns {res, rem}
	if (m < n) return {{}, a}; // if min(m-n,n) < 750 it may be
	Poly ap = a, bp = b;       // faster to use cuadratic version
	reverse(ALL(ap));
	reverse(ALL(bp));
	bp = inv(bp, m - n + 1);
	Poly q = conv(ap, bp);
	q.resize(SZ(q) + m - n - SZ(q) + 1, 0);
	reverse(ALL(q));
	Poly bq = conv(b, q);
	fore(i,0,SZ(bq)) if (bq[i]) bq[i] = mod - bq[i];
	Poly r = add(a, bq);
	return {q, r};
}

void filltree(vi& x, vector<Poly>& tree) {
	ll k = SZ(x);
	tree.resize(2*k);
	fore(i, k, 2*k) tree[i] = {(mod - x[i - k]) % mod, 1};
	for(ll i = k; i--;) tree[i] = conv(tree[2*i], tree[2*i+1]);
}
vi evaluate(Poly& a, vi& x) { // O(n log(n)^2)
	vector<Poly> tree;          // Evaluate a in all points of x
	filltree(x, tree);
	ll k = SZ(x);
	vector<Poly> ans(2*k);
	ans[1] = div(a, tree[1]).snd;
	fore(i,2,2*k) ans[i] = div(ans[i>>1], tree[i]).snd;
	vi r;
	fore(i,0,k) r.pb(ans[i+k][0]);
	return r;
}
Poly interpolate(vi& x, vi& y) { // O(n log(n)^2)
	vector<Poly> tree;
	filltree(x, tree);
	Poly p = derivate(tree[1]);
	ll k = SZ(y);
	vi d = evaluate(p, x);
	vector<Poly> intree(2*k);
	fore(i, k, 2*k) intree[i] = {y[i-k] * inv(d[i-k])};
	for(ll i = k-1; i; i--) {
		Poly p1 = conv(tree[2*i], intree[2*i+1]);
		Poly p2 = conv(tree[2*i+1], intree[2*i]);
		intree[i] = add(p1, p2);
	}
	return intree[1];
}
