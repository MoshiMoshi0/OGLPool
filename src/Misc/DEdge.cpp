/*
 * DEdge.cpp
 *
 *  Created on: 23-03-2013
 *      Author: _CORE7
 */

#include "DEdge.h"

namespace OGLPool {
	DEdge::DEdge() : s(0), t(0), l(0), r(0) {}
	DEdge::DEdge(int s, int t) : s(s), t(t), l(0), r(0) {}
	DEdge::DEdge(int s, int t, int l, int r) : s(s), t(t), l(l), r(r) {}
} /* namespace OGLPool */
