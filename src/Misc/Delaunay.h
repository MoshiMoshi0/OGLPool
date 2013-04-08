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

	void triangulate( const vector< vec2 >& points );
	void triangulate( const vector< vec2 >& points, vector< Triangle2 >& tris );
	void triangulate( const vector< vec3 >& points, vector< Triangle3 >& tris );
	vector< DEdge > getDEdges();
	vector< Edge2 > getEdges();
	vector< vec2 > getPoints();
	vector< Triangle2 > getTriangles();
	vector< ivec3 > getTriangleIndices();
	bool isValid();

	enum DEdgeType {
		UNDEFINED = -2,
		UNIVERSE = -1,
	};
private:
	enum class DState { VALID, INVALID };

	int addEdge( int s, int t );
	int addEdge( int s, int t, int l, int r );
	int findEdge(int s, int t);
	void updateLeftFace(int eI, int s, int t, int f);
	void completeFacet(int eI);
	void findClosestNeighbours( int& u, int& v);

	vector< DEdge > edges;
	vector< vec2 > points;
	int nFaces;
	DState state;
	unsigned int u, s, t, bP;
};

} /* namespace OGLPool */
#endif /* DELAUNAY_H_ */
