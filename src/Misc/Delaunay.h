/*
 * Delaunay.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef DELAUNAY_H_
#define DELAUNAY_H_

#include <vector>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <Misc/DEdge.h>

using namespace glm;
using namespace std;

namespace OGLPool {

class Delaunay {
public:
	enum {
		UNDEFINED = -1,
		UNIVERSE = 0
	};

	Delaunay();
	virtual ~Delaunay();

	void triangulate( vector< vec2 > points );

	//void addTriangle( int s, int t, int u );
	int addEdge( int s, int t );
	int addEdge( int s, int t, int l, int r );
	int findEdge(int s, int t);
	void updateLeftFace(int eI, int s, int t, int f);
	void completeFacet(int eI, int nFaces);
	void findClosestNeighbours( int& u, int& v);
	vector< ivec3 > constructTris();
	void error( string s ){
		cout << s << endl;
		exit( 1 );
	}

	vector< DEdge > edge;
	vector< vec2 > point;
};

} /* namespace OGLPool */
#endif /* DELAUNAY_H_ */
