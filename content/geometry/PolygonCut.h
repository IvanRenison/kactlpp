/**
 * Author: Ulf Lundstrom
 * Date: 2009-03-21
 * License: CC0
 * Source:
 * Description:
 * \descriptionimage{content/geometry/PolygonCut}
 * Returns a vector with the vertices of a polygon with everything to the left of the line
 * going from s to e cut away.
 * Usage:
 * 	vector<P> p = ...;
 * 	p = polygonCut(p, P(0,0), P(1,0));
 * Status: tested but not extensively
 */
#pragma once

#include "Point.h"

typedef Point<double> P;
vector<P> polygonCut(const vector<P>& poly, P s, P e) {
	vector<P> res;
	fore(i,0,SZ(poly)) {
		P cur = poly[i], prev = i ? poly[i-1] : poly.back();
		auto a = s.cross(e, cur), b = s.cross(e, prev);
		if ((a < 0) != (b < 0))
			res.pb(cur + (prev - cur) * (a / (a - b)));
		if (a < 0) res.pb(cur);
	}
	return res;
}
