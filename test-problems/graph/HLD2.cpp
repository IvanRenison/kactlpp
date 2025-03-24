// Problem: https://codeforces.com/contest/2007/problem/E
// Status: Accepted
// Submission: https://codeforces.com/contest/2007/submission/312253506
// Testing usage of normal segment tree (not lazy) for HLD with values in edges
#include <bits/stdc++.h>
using namespace std;

#define fst first
#define snd second
#define pb push_back
#define fore(i, a, b) for (ll i = a, gmat = b; i < gmat; i++)
#define ALL(x) x.begin(), x.end()
#define SZ(x) (ll)(x).size()
#define mset(a, v) memset((a), (v), sizeof(a))
typedef long long ll;
typedef pair<ll, ll> ii;
typedef vector<ll> vi;

/// content/data-structures/SegmentTree.h
/// START diff
typedef pair<bool, ll> T;
static constexpr T neut = {true, 0};
T f(T a, T b) { return {a.fst && b.fst, a.snd + b.snd}; } // (any associative fn)
struct Tree {
	/// END diff
	vector<T> s; ll n;
	Tree(ll n = 0, T def = neut) : s(2*n, def), n(n) {}
	void upd(ll pos, T val) {
		for (s[pos += n] = val; pos /= 2;)
			s[pos] = f(s[pos * 2], s[pos * 2 + 1]);
	}
	T query(ll b, ll e) { // query [b, e)
		T ra = neut, rb = neut;
		for (b += n, e += n; b < e; b /= 2, e /= 2) {
			if (b % 2) ra = f(ra, s[b++]);
			if (e % 2) rb = f(s[--e], rb);
		}
		return f(ra, rb);
	}
};
/// END content

/// content/graph/HLD.h
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
	/// START diff
	// void updPath(ll u, ll v, L val) {
	// 	process(u, v, [&](ll l, ll r) { t.upd(l, r, val); });
	// }
	/// END diff
	T queryPath(ll u, ll v) {
		/// START diff
		T res = neut;
		/// END diff
		process(u, v, [&](ll l, ll r) {
				res = f(res, t.query(l, r));
		});
		return res;
	}
	T querySubtree(ll v) { // updSubtree is similar
		return t.query(pos[v] + VALS_ED, pos[v] + siz[v]);
	}
	/// START diff
	void updPoint(ll v, T val) {
		t.upd(pos[v], val);
	}
	/// END diff
};
/// END content

void solve() {
	ll n, w;
	cin >> n >> w;
	vector<vi> adj(n);
	fore(i, 1, n) {
		ll p;
		cin >> p;
		p--;
		adj[p].pb(i), adj[i].pb(p);
	}

	vi sub_tree_maxs(n);
	function<ll(ll, ll)> dfs = [&](ll u, ll p) {
		ll ans = u;
		for (ll v : adj[u]) if (v != p) {
			ans = max(ans, dfs(v, u));
		}
		sub_tree_maxs[u] = ans;
		return  ans;
	};
	dfs(0, -1);


	HLD<true> hld(adj);
	fore(u, 1, n) {
		hld.updPoint(u, {false, 0});
	}

	ll free = n; // amount of paths that are not fully defined
	ll defined_ans = 0; // sum of defined parts of paths
	fore(_, 0, n - 1) {
		ll u, y;
		cin >> u >> y;
		u--;

		hld.updPoint(u, {true, y});
		w -= y;
		defined_ans += 2 * y;

		ll v = sub_tree_maxs[u];
		assert(u <= v);

		{ // define path u-1 u
			assert(u > 0);
			ll u_ = u - 1;
			auto [defined, val] = hld.queryPath(u_, u);

			if (defined) {
				free--;
			}
		}
		{ // define path v v+1
			ll v_ = (v + 1) % n;
			auto [defined, val] = hld.queryPath(v, v_);

			if (defined) {
				free--;
			}
		}

		ll ans = defined_ans + free * w;
		cout << ans << ' ';
	}

	cout << '\n';
}

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll t;
	cin >> t;
	while (t--) {
		solve();
	}
}
