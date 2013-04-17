/*
 * SmoothRandomPolygon.h
 *
 *  Created on: 13-04-2013
 *      Author: DarkNeo
 */

#ifndef SMOOTHRANDOMPOLYGON_H_
#define SMOOTHRANDOMPOLYGON_H_

#include "RandomPolygon.h"

namespace OGLPool {

class SmoothRandomPolygon: public OGLPool::RandomPolygon {
public:
	enum Type {
		ROUND, RANDOM
	};

	SmoothRandomPolygon( Type type );
	virtual ~SmoothRandomPolygon();

	void draw();

	vector<Edge2> bezierEdges;
private:
	void createRandom();
	void createRound();
	void getBezierPoints( const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, vector<vec2>& bezierPts, uint quality );
	vector< float > tridiagonalSolve(const vector< float >& a, const vector< float >& b, const vector< float >& c, const vector< float >& rhs, uint n);
	vector< float > cyclicSolve( const vector< float >& a, const vector< float >& b, const vector< float >& c, float alpha, float beta, const vector< float >& rhs, uint n );

	void smoothEdge(int randEdge);
};

} /* namespace OGLPool */
#endif /* SMOOTHRANDOMPOLYGON_H_ */
