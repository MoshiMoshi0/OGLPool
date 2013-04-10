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
	vector < DEdge >getBoundary(vector < DEdge > des);
	int getTriangleUniverseEdgeCount( int triIndex, vector < DEdge > des );
	void removeUniverseEdges(int triIndex, int universeEdgeCount, vector < DEdge >& des );
};

} /* namespace OGLPool */
#endif /* RANDOMPOLYGON_H_ */
