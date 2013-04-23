/*
 * RandomPolygon.h
 *
 *  Created on: 09-04-2013
 *      Author: _CORE7
 */

#ifndef RANDOMPOLYGON_H_
#define RANDOMPOLYGON_H_

#include "Polygon.h"
#include <vector>
#include <Misc/DEdge.h>
using namespace std;

namespace OGLPool {

class RandomPolygon: public OGLPool::Polygon {
public:
	RandomPolygon( uint numSides, uint numPoints );
	virtual ~RandomPolygon();

private:
	void removeUniverseEdges( int triIndex, vector<DEdge>& edges );
	void buildPolygon(const vector<DEdge>& boundary, const vector<vec2>& rndPoints);
	bool canRemove( int triIndex, vector<DEdge> edges );
	bool fixSides(vector<DEdge>& edges, uint numSides);
	bool removeEdges( int universeCount, const vector<DEdge>& boundary, vector<DEdge>& edges );
	bool validateBoundary( const vector<DEdge>& boundary, const vector<vec2>& points, const float& minAng, const float& minLen );
	int getRandomTriIndex(const vector<DEdge>& edges);
	int getUniverseEdgeCount( int triIndex, const vector<DEdge>& edges );
	vector<DEdge> getBoundary( const vector<DEdge>& edges );
	vector<DEdge> sortBoundary(const vector<DEdge>& boundary);
	DEdge getNextEdge(const vector<DEdge>& boundary, const DEdge& current);
	void generate(uint numPoints, uint numSides);
};

} /* namespace OGLPool */
#endif /* RANDOMPOLYGON_H_ */
