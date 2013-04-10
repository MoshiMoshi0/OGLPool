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
	int getRandomTriIndex(vector<DEdge> x);
	int getUniverseEdgeCount( int triIndex, vector < DEdge > edges );
	void removeUniverseEdges( int triIndex, vector < DEdge >& edges );
	bool removeEdges( int universeCount, vector<DEdge> boundary, vector<DEdge>& edges );
	vector< DEdge > getBoundary( vector < DEdge > edges );
};

} /* namespace OGLPool */
#endif /* RANDOMPOLYGON_H_ */
