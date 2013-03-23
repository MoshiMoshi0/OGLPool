/*
 * Delaunay.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef DELAUNAY_H_
#define DELAUNAY_H_

#include <vector>
#include <glm/glm.hpp>
#include <Misc/DEdge.h>
#include <Shape/Edge.h>
#include <Shape/Triangle.h>

using namespace glm;
using namespace std;

namespace OGLPool {

class Delaunay {
public:
	Delaunay();
	virtual ~Delaunay();

	void triangulate( vector< vec2 > points );
	vector< DEdge > getDEdges();
	vector< Edge2 > getEdges();
	vector< vec2 > getPoints();
	vector< Triangle2 > getTriangles();
	vector< ivec3 > getTriangleIndices();

private:
	enum {
		UNDEFINED = -1,
		UNIVERSE = 0
	};

	int addEdge( int s, int t );
	int addEdge( int s, int t, int l, int r );
	int findEdge(int s, int t);
	void updateLeftFace(int eI, int s, int t, int f);
	void completeFacet(int eI, int nFaces);
	void findClosestNeighbours( int& u, int& v);

	vector< DEdge > edges;
	vector< vec2 > points;
};

} /* namespace OGLPool */
#endif /* DELAUNAY_H_ */
