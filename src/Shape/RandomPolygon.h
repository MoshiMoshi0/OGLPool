/*
 * RandomPolygon.h
 *
 *  Created on: 09-04-2013
 *      Author: _CORE7
 */

#ifndef RANDOMPOLYGON_H_
#define RANDOMPOLYGON_H_

#include "Polygon.h"
#include <Misc/DEdge.h>
#include <vector>
using namespace std;

namespace OGLPool {

class RandomPolygon: public OGLPool::Polygon {
public:
	RandomPolygon();
	virtual ~RandomPolygon();

private:
	void removeUniverseEdges( int triIndex, vector < DEdge >& edges );
	bool canRemove( int triIndex, vector<DEdge> edges );
	bool generate(vector< DEdge >& edges, uint numSides);
	bool removeEdges( int universeCount, const vector< DEdge >& boundary, vector<DEdge>& edges );
	int getRandomTriIndex(const vector< DEdge >& edges);
	int getUniverseEdgeCount( int triIndex, const vector< DEdge >& edges );
	vector< DEdge > getBoundary( const vector< DEdge >& edges );
};

} /* namespace OGLPool */
#endif /* RANDOMPOLYGON_H_ */
