/**
 * Author: Simon Lindholm
 * Date: 2017-04-17
 * License: CC0
 * Source: folklore
 * Description: Finds all biconnected components in an undirected graph. In a
 *  biconnected component there are at least two internally disjoint paths
 *  between any two nodes (a cycle exists through them). Note that a node can
 *  be in several components. An edge which is not in a component is a bridge,
 *  i.e., not part of any cycle.
 * Time: O(E + V)
 * Status: tested with library checker and with SPOJ EC_P
 */
#pragma once

auto BCC(ll n, const vector<ii>& edges) {
	ll Time = 0, eid = 0;
	vi num(n), st;
	vector<vector<ii>> adj(n);
	for (auto [a, b] : edges)
		adj[a].pb({b, eid}), adj[b].pb({a, eid++});

	ll nComps = 0; // number of biconnected components
	vi edgesComp(eid, -1); // comp of each edge or -1 if bridge
	vector<set<ll>> nodesComp(n); // comps of each node

	function<ll(ll, ll)> dfs = [&](ll at, ll par) {
		ll me = num[at] = ++Time, top = me;
		for (auto [y, e] : adj[at]) if (e != par) {
			if (y == at) { // self loop
				nodesComp[at].insert(edgesComp[e] = nComps++);
			} else if (num[y]) {
				top = min(top, num[y]);
				if (num[y] < me) st.pb(e);
			} else {
				ll si = SZ(st), up = dfs(y, e);
				top = min(top, up);
				if (up == me) {
					st.pb(e); // from si to SZ(st) we have a comp
					fore(i, si, SZ(st)) {
						edgesComp[st[i]] = nComps;
						auto [u, v] = edges[st[i]];
						nodesComp[u].insert(nComps);
						nodesComp[v].insert(nComps);
					}
					nComps++, st.resize(si);
				} else if (up < me) st.pb(e); // else e is bridge
			}
		}
		return top;
	};

	fore(u, 0, n) if (!num[u]) dfs(u, -1);
	fore(u, 0, n) if (nodesComp[u].empty())
		nodesComp[u].insert(nComps++);

	return tuple(nComps, edgesComp, nodesComp);
};
