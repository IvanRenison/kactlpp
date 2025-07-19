#include "../utilities/template.h"
#include "../../content/data-structures/PersistentSegmentTree.h"


namespace Slow {

#include "../../content/data-structures/SegmentTree.h"

struct Slow {
	ll n;
	vector<Tree> trees;

	Slow(ll n) : n(n), trees(1, Tree(n)) {}
	ll init(const vector<Tree::T>& v) {
		assert(SZ(v) == n);
		trees.pb(Tree(n));
		fore(i, 0, n) {
			trees.back().upd(i, v[i]);
		}
		return SZ(trees) - 1;
	}
	ll upd(ll ver, ll i, Tree::T x) {
		assert(ver < SZ(trees));
		trees.pb(trees[ver]);
		trees.back().upd(i, x);
		return SZ(trees) - 1;
	}

	Tree::T query(ll ver, ll l, ll r) {
		assert(ver < SZ(trees));
		return trees[ver].query(l, r);
	}
};

}

int main() {
	fore(_, 0, 100) {
		ll n = rand() % 100 + 1;

		Slow::Slow slow(n);
		Tree st(n);

		vi slow_versions = {0};
		vi st_versions = {0};

		fore(q, 0, 100) {
			ll ver = rand() % SZ(slow_versions);

			ll t = rand() % 3;
			if (t == 0) { // init
				vector<ll> v(n);
				fore(i, 0, n) {
					v[i] = rand() % 1000;
				}

				slow_versions.pb(slow.init(v));
				st_versions.pb(st.init(v));
			} else if (t == 1) { // Update
				ll i = rand() % n;
				ll x = rand() % 10000;

				slow_versions.pb(slow.upd(slow_versions[ver], i, x));
				st_versions.pb(st.upd(st_versions[ver], i, x));
			} else { // Query
				ll l = rand() % n;
				ll r = rand() % (n + 1);
				if (l > r) swap(l, r);

				ll slow_ans = slow.query(slow_versions[ver], l, r);
				ll st_ans = st.query(st_versions[ver], l, r);

				assert(slow_ans == st_ans);
			}
		}

		assert(slow.query(slow_versions.back(), 0, n) == st.query(st_versions.back(), 0, n));
	}

	cout << "Tests passed!" << endl;
}
