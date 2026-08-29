/**
 * Author: Johan Sannemo
 * Date: 2016-12-15
 * License: CC0
 * Source: automaton from notebook el vasito
 * Description: pi[x] computes the length of the longest prefix of s that ends at x,
 * other than s[0...x] itself (abacaba -> 0010123).
 * Can be used to find all occurrences of a string.
 * Time: O(n) for KMP, O(n * alpha) for automaton
 * Status: Tested on kattis:stringmatching, automaton untested
 */
#pragma once

vi pi(const string& s) {
	vi p(SZ(s));
	fore(i,1,SZ(s)) {
		ll g = p[i-1];
		while (g && s[i] != s[g]) g = p[g-1];
		p[i] = g + (s[i] == s[g]);
	}
	return p;
}

vi match(const string& s, const string& pat) {
	vi p = pi(pat + '\0' + s), res;
	fore(i,SZ(p)-SZ(s),SZ(p))
		if (p[i] == SZ(pat)) res.pb(i - 2 * SZ(pat));
	return res;
}

const ll alpha = 26;
vector<vi> buildAutomaton(string &s) {
	vector<vi> wh(SZ(s) + 2, vi(alpha));
	ll lps = 0; wh[0][s[0] - 'a'] = 1;
	fore(i, 1, SZ(s) + 1) {
		fore(j, 0, alpha) wh[i][j] = wh[lps][j];
		if(i<SZ(s)) wh[i][s[i]-'a'] = i+1, lps = wh[lps][s[i]-'a'];
	}
}
