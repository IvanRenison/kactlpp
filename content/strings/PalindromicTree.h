/**
 * Author: Unknown
 * Date: 2024-11-05
 * License: CC0
 * Source: notebook el vasito
 * Status: untested
 */

#pragma once

struct PalindromicTree {
	static const ll alpha = 26;
	struct Node {
		ll len, link = 0, cnt = 1;
		array<ll, alpha> to{};
	};
	vector<Node> ns;
	ll last;
	PalindromicTree() : last(0) {ns.pb({-1}); ns.pb({0});}
	void add(ll i, string &s) {
		ll p = last, c = s[i] - 'a';
		while (s[i - ns[p].len - 1] != s[i]) p = ns[p].link;
		if (ns[p].to[c]) last = ns[p].to[c], ns[last].cnt++;
		else {
			ll q = ns[p].link;
			while (s[i - ns[q].len - 1] != s[i]) q = ns[q].link;
			last = ns[p].to[c] = SZ(ns);
			ns.pb({ns[p].len + 2, max(1ll, ns[q].to[c]), 1});
		}
	}
};
