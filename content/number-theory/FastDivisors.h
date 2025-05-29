/**
 * Author: Pietro Palombini
 * Date: 2024-10-11
 * License: CC0
 * Description: Given the prime factorization of a number,
 * returns all its divisors. Yo can use Eratosthenes to get the
 * prime factorization.
 * Time: O(d), where $d$ is the number of divisors.
 * Status: stress-tested
 */

#pragma once

vi divisors(vector<ii>& f) {
	vi res = {1};
	for (auto& [p, k] : f) {
		ll sz = SZ(res);
		fore(i,0,sz) for(ll j=0,x=p;j<k;j++,x*=p) res.pb(res[i]*x);
	}
	return res;
}
