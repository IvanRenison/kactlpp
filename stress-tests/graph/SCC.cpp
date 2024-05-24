#include "../utilities/template.h"

#include "../../content/graph/SCC.h"

namespace old {
vi orig, low, comp, z;
ll no_vertices, no_components;
template<class G> void dfs(ll j, G &g) {
	low[j] = orig[j] = no_vertices++;
	comp[j] = -2; z.pb(j);
	for(auto &e:g[j])
		if (comp[e] == -1) {
			dfs(e, g);
			low[j] = min(low[j], low[e]);
		}
		else if (comp[e] == -2)
			low[j] = min(low[j], orig[e]);

	if (orig[j] == low[j]) {
		for (;;) {
			ll x = z.back(); z.pop_back();
			comp[x] = no_components;
			if (x == j) break;
		}
		no_components++;
	}
}
template<class G> vi scc(G &g) {
	ll n = SZ(g);
	orig.assign(n, 0); low = orig;
	no_vertices = no_components = 0;
	comp.assign(n, -1);
	fore(i,0,n) if (comp[i] == -1) dfs(i, g);
	return comp;
}
}

int main() {
	unsigned r = 1;
	for (ll N = 0; N <= 4; N++) {
		// cout << "N = " << N << endl;
		vector<vi> mat(N, vi(N)), adj(N);
		vi compsize(N), seen(N);
		ll count = 0;
		fore(bits,0,(1 << (N*N))) {
			// if (bits % 10000 == 0) cerr << "." << flush;
			fore(i,0,N) fore(j,0,N)
				mat[i][j] = bits & 1 << (i*N+j);

			fore(i,0,N) {
				adj[i].clear();
				fore(j,0,N) if (bits & 1 << (i*N+j)) {
					adj[i].pb(j);
					r *= 12387123; r += 1231;
					if ((r >> 6 & 31) == 3)
						adj[i].pb(j);
				}
			}
			vi comp2 = old::scc(adj);
			scc(adj, [&](vi& v) {
				compsize[ncomps] = SZ(v);
			});
			if (comp != comp2) {
				for(auto &x: comp) cout << x << ' ';
				cout << endl;
				for(auto &x: comp2) cout << x << ' ';
				cout << endl;
			}
			fore(i,0,N) assert(comp[i] >= 0 && comp[i] < ncomps);
			fore(i,0,N) for(auto &j: adj[i]) assert(comp[j] <= comp[i]);
			fore(i,0,N) {
				seen.assign(N, 0); seen[i] = 1;
				fore(it,0,N) {
					fore(j,0,N) if (seen[j]) for(auto &k: adj[j]) seen[k] = 1;
				}
				fore(j,0,N) {
					if (seen[j]) assert(comp[j] <= comp[i]);
					else assert(comp[j] != comp[i]);
				}
			}

			count++;
		}
		// cout << "tested " << count << endl;
	}
	cout<<"Tests passed!"<<endl;
	return 0;
}
