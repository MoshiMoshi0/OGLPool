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

class SmoothRandomPolygon: public RandomPolygon {
public:
	enum Type {
		ROUND, RANDOM
	};

	SmoothRandomPolygon();
	virtual ~SmoothRandomPolygon();

	void draw();
	bool generate(Type type, uint numPoints, uint numSides, float scale, float tightness = 1.0f, uint numTries = 1000);

	vector<Edge2> bezierEdges;
	vector<Edge2> bezierControlEdges;
private:
	bool generate(uint numPoints, uint numSides, float scale, uint numTries = 1000);
	void createRandom();
	void createRound();
	vector< float > tridiagonalSolve(const vector< float >& a, const vector< float >& b, const vector< float >& c, const vector< float >& rhs, uint n);
	vector< float > cyclicSolve( const vector< float >& a, const vector< float >& b, const vector< float >& c, float alpha, float beta, const vector< float >& rhs, uint n );

	void smoothEdge(int randEdge);

	float tightness;
};

} /* namespace OGLPool */
#endif /* SMOOTHRANDOMPOLYGON_H_ */
