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
#include <Util/Triangulation/DEdge.h>
using namespace std;

namespace OGLPool {

class RandomPolygon : public Polygon2 {
public:
	RandomPolygon();
	virtual ~RandomPolygon();

	virtual bool generate(uint numPoints, uint numSides, float scale, uint numTries = 1000);
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
};

} /* namespace OGLPool */
#endif /* RANDOMPOLYGON_H_ */
