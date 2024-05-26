// Problem: https://judge.yosupo.jp/problem/dynamic_tree_vertex_add_path_sum
// Status:
// Submission:
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
typedef pair<ll, ll> pii;
typedef vector<ll> vi;

/// content/graph/LinkCutTree.h
const ll N_DEL = 0, N_VAL = 0; //delta, value
inline ll mOp(ll x, ll y){return x+y;}//modify
inline ll qOp(ll lval, ll rval){return lval + rval;}//query
inline ll dOnSeg(ll d, ll len){return d==N_DEL ? N_DEL : d*len;}
//mostly generic
inline ll joinD(ll d1, ll d2){
	if(d1==N_DEL)return d2;
	if(d2==N_DEL)return d1;
	return mOp(d1, d2);
}
inline ll joinVD(ll v, ll d){return d==N_DEL ? v : mOp(v, d);}

struct LinkCutTree {
	struct Node {
		ll sz_, nVal, tVal, d;
		bool rev;
		ll c[2], p;
		Node(ll v = N_VAL) : sz_(1), nVal(v), tVal(v), d(N_DEL), rev(0), p(-1){
			c[0] = c[1] = -1;
		}
	};
	ll n;
	vector<Node> nodes;

	LinkCutTree(ll n) : n(n), nodes(n) {
		fore(i, 0, n) {
			nodes[i] = Node(N_VAL);
		}
	}
	LinkCutTree(vi& vals) : n(vals.size()), nodes(n) {
		fore(i, 0, n) {
			nodes[i] = Node(vals[i]);
		}
	}

	bool isRoot(ll u) {
		return nodes[u].p == -1 || (nodes[nodes[u].p].c[0] != u && nodes[nodes[u].p].c[1] != u);
	}
	void push(ll u) {
		if (nodes[u].rev) {
			nodes[u].rev=0;
			swap(nodes[u].c[0], nodes[u].c[1]);
			fore(x, 0, 2) if (nodes[u].c[x] != -1) nodes[nodes[u].c[x]].rev ^= 1;
		}
		nodes[u].nVal = joinVD(nodes[u].nVal, nodes[u].d);
		nodes[u].tVal = joinVD(nodes[u].tVal, dOnSeg(nodes[u].d, nodes[u].sz_));
		fore(x, 0, 2) if(nodes[u].c[x] != -1)
			nodes[nodes[u].c[x]].d = joinD(nodes[nodes[u].c[x]].d, nodes[u].d);
		nodes[u].d=N_DEL;
	}
	ll getPV(ll u) {
		return joinVD(nodes[u].tVal, dOnSeg(nodes[u].d, nodes[u].sz_));
	}
	void upd(ll u) {
		nodes[u].tVal = qOp(
			qOp((nodes[u].c[0] != -1 ? getPV(nodes[u].c[0]) : N_VAL), joinVD(nodes[u].nVal, nodes[u].d)),
			(nodes[u].c[1] != -1 ? getPV(nodes[u].c[1]) : N_VAL));
		nodes[u].sz_ = 1 + (nodes[u].c[0] != -1 ? nodes[nodes[u].c[0]].sz_ : 0) + (nodes[u].c[1] != -1 ? nodes[nodes[u].c[1]].sz_ : 0);
	}

	void conn(ll c, ll p, ll il) {
		if (c != -1) nodes[c].p = p;
		if (il >= 0) nodes[p].c[!il] = c;
	}
	void rotate(ll u) {
		ll p = nodes[u].p, g = nodes[p].p;
		bool gCh = isRoot(p), isl = u == nodes[p].c[0];
		conn(nodes[u].c[isl], p, isl); conn(p, u, !isl);
		conn(u, g, gCh ? -1 : (p == nodes[g].c[0]));
		upd(p);
	}
	void spa(ll u) { // splay
		while(!isRoot(u)){
			ll p = nodes[u].p, g = nodes[p].p;
			if(!isRoot(p)) push(g);
			push(p), push(u);
			if(!isRoot(p)) rotate((u == nodes[p].c[0]) == (p == nodes[g].c[0]) ? p : u);
			rotate(u);
		}
		push(u), upd(u);
	}
	ll lift_rec(ll u, ll t) {
		if(u == -1) return 0;
		if(t == (nodes[u].c[0] != -1 ? nodes[nodes[u].c[0]].sz_ : 0)) {
			spa(u);
			return u;
		}
		if(t < (nodes[u].c[0] != -1 ? nodes[nodes[u].c[0]].sz_ : 0))
			return lift_rec(nodes[u].c[0], t);
		return lift_rec(nodes[u].c[1], t - (nodes[u].c[0] != -1 ? nodes[nodes[u].c[0]].sz_ : 0) - 1);
	}
	ll exv(ll u){ // expose
		ll last = -1;
		for(ll v = u; v != -1; v = nodes[v].p)
			spa(v), nodes[v].c[0] = last, upd(v), last = v;
		spa(u);
		return last;
	}

	void mkR(ll u){ // makeRoot
		exv(u);
		nodes[u].rev ^= 1;
	}
	ll getR(ll u){
		exv(u);
		while(nodes[u].c[1]) u = nodes[u].c[1];
		spa(u);
		return u;
	}
	ll lca(ll u, ll v) {
		exv(u);
		return exv(v);
	}
	bool connected(ll u, ll v) {
		exv(u), exv(v);
		return u == v ? true : nodes[u].p != 0;
	}
	void link(ll u, ll v) {
		mkR(u);
		nodes[u].p = v;
	}
	void cut(ll u, ll v) {
		mkR(u), exv(v);
		nodes[nodes[v].c[1]].p = -1, nodes[v].c[1] = -1;
	}
	ll father(ll u){
		exv(u);
		ll v = nodes[u].c[1];
		if (v == -1) return 0;
		while(nodes[v].c[0] != -1) v = nodes[v].c[0];
		return v;
	}
	void cut(ll u) { // cuts x from father keeping tree root
		exv(u);
		nodes[u].p = -1;
	}
	ll query(ll u, ll v) {
		mkR(u), exv(v);
		return getPV(v);
	}
	void modify(ll u, ll v, ll d) {
		mkR(u), exv(v);
		nodes[v].d = joinD(nodes[v].d, d);
	}
	ll depth(ll u) { // distance from x to its tree root
		exv(u);
		return nodes[u].sz_ - 1;
	}
	ll lift(ll u, ll t) { // t-th ancestor of x (lift(x,1) is x's father)
		exv(u);
		return lift_rec(u, t);
	}
};
///END content

int main() {
	cin.tie(0)->sync_with_stdio(0);

	ll N, Q;
	cin >> N >> Q;

	LinkCutTree lct(N);
	fore(i, 0, N) {
		ll a;
		cin >> a;
		lct.modify(i, i, a);
	}
	fore(i, 0, N - 1) {
		ll a, b;
		cin >> a >> b;
		lct.link(a, b);
	}

	while (Q--) {
		ll t;
		cin >> t;
		if (t == 0) {
			ll u, v, w, x;
			cin >> u >> v >> w >> x;
			lct.cut(u, v);
			lct.link(w, x);
		} else if (t == 1) {
			ll p, x;
			cin >> p >> x;
			lct.modify(p, p, x);
		} else {
			ll u, v;
			cin >> u >> v;
			ll val = lct.query(u, v);
			cout << val << '\n';
		}
	}
}
