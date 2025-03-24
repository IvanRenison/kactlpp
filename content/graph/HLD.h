/**
 * Author: Benjamin Qi, Oleksandr Kulkov, chilli
 * Date: 2020-01-12
 * License: CC0
 * Source: https://codeforces.com/blog/entry/53170, https://github.com/bqi343/USACO/blob/master/Implementations/content/graphs%20(12)/Trees%20(10)/HLD%20(10.3).h
 * Description: Decomposes a tree into vertex disjoint heavy paths and light
 * edges such that the path from any leaf to the root contains at most log(n)
 * light edges. Code does additive modifications and sum queries, but can
 * support commutative segtree modifications/queries on paths and subtrees.
 * For a non-commutative operation see LinkCutTree.
 * Takes as input the full adjacency list. VALS\_ED being true means that
 * values are stored in the edges, as opposed to the nodes, and updates
 * have to be done on child nodes. All values initialized to the segtree default.
 * Root must be 0. If you only have point updates you can use normal segment tree instead of lazy.
 * Time: O((\log N)^2)
 * Status: stress-tested a bit
 */
#pragma once

#include "../data-structures/LazySegmentTree.h"

template <bool VALS_ED> struct HLD {
	ll N, tim = 0;
	vector<vi> adj;
	vi par, siz, rt, pos;
	Tree t;
	HLD(vector<vi> adj_)
		: N(SZ(adj_)), adj(adj_), par(N, -1), siz(N, 1),
		  rt(N), pos(N), t(N) { dfsSz(0), dfsHld(0); }
	void dfsSz(ll v) {
		for (ll& u : adj[v]) {
			adj[u].erase(find(ALL(adj[u]), v));
			par[u] = v;
			dfsSz(u);
			siz[v] += siz[u];
			if (siz[u] > siz[adj[v][0]]) swap(u, adj[v][0]);
		}
	}
	void dfsHld(ll v) {
		pos[v] = tim++;
		for (ll u : adj[v]) {
			rt[u] = (u == adj[v][0] ? rt[v] : u);
			dfsHld(u);
		}
	}
	void process(ll u, ll v, auto op) {
		for (;; v = par[rt[v]]) {
			if (pos[u] > pos[v]) swap(u, v);
			if (rt[u] == rt[v]) break;
			op(pos[rt[v]], pos[v] + 1);
		}
		op(pos[u] + VALS_ED, pos[v] + 1);
	}
	void updPath(ll u, ll v, L val) {
		process(u, v, [&](ll l, ll r) { t.upd(l, r, val); });
	}
	T queryPath(ll u, ll v) {
		T res = tneut;
		process(u, v, [&](ll l, ll r) {
				res = f(res, t.query(l, r));
		});
		return res;
	}
	T querySubtree(ll v) { // updSubtree is similar
		return t.query(pos[v] + VALS_ED, pos[v] + siz[v]);
	}
	// void updPoint(ll v, T val) { // For normal segment tree
	// 	t.upd(pos[v], val); // queryPoint is similar
	// }
};
