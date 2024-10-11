/**
 * Author: chilli, SJTU, Janez Konc
 * Date: 2019-05-10
 * License: GPL3+
 * Source: https://en.wikipedia.org/wiki/MaxCliqueDyn_maximum_clique_algorithm, https://gitlab.com/janezkonc/mcqd/blob/master/mcqd.h
 * Description: Quickly finds a maximum clique of a graph (given as symmetric bitset
 * matrix; self-edges not allowed). Can be used to find a maximum independent
 * set by finding a clique of the complement graph.
 * Time: Runs in about 1s for n=155 and worst case random graphs (p=.90). Runs
 * faster for sparse graphs.
 * Status: stress-tested
 */
typedef vector<bitset<200>> vb;
struct Maxclique {
	double limit=0.025, pk=0;
	struct Vertex { ll i, d=0; };
	typedef vector<Vertex> vv;
	vb e;
	vv V;
	vector<vi> C;
	vi qmax, q, S, old;
	void init(vv& r) {
		for (auto& v : r) v.d = 0;
		for (auto& v : r) for (auto j : r) v.d += e[v.i][j.i];
		sort(ALL(r), [](auto a, auto b) { return a.d > b.d; });
		ll mxD = r[0].d;
		fore(i,0,SZ(r)) r[i].d = min(i, mxD) + 1;
	}
	void expand(vv& R, ll lev = 1) {
		S[lev] += S[lev - 1] - old[lev];
		old[lev] = S[lev - 1];
		while (SZ(R)) {
			if (SZ(q) + R.back().d <= SZ(qmax)) return;
			q.pb(R.back().i);
			vv T;
			for (auto v : R) if (e[R.back().i][v.i]) T.pb({v.i});
			if (SZ(T)) {
				if (S[lev]++ / ++pk < limit) init(T);
				ll j = 0, mxk = 1, mnk = max(SZ(qmax) - SZ(q) + 1,1ll);
				C[1].clear(), C[2].clear();
				for (auto v : T) {
					ll k = 1;
					auto f = [&](ll i) { return e[v.i][i]; };
					while (any_of(ALL(C[k]), f)) k++;
					if (k > mxk) mxk = k, C[mxk + 1].clear();
					if (k < mnk) T[j++].i = v.i;
					C[k].pb(v.i);
				}
				if (j > 0) T[j - 1].d = 0;
				fore(k,mnk,mxk + 1) for (ll i : C[k])
					T[j].i = i, T[j++].d = k;
				expand(T, lev + 1);
			} else if (SZ(q) > SZ(qmax)) qmax = q;
			q.pop_back(), R.pop_back();
		}
	}
	vi maxClique() { init(V), expand(V); return qmax; }
	Maxclique(vb conn) : e(conn), C(SZ(e)+1), S(SZ(C)), old(S) {
		fore(i,0,SZ(e)) V.pb({i});
	}
};
