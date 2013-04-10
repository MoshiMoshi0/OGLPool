/*
 * Polygon.h
 *
 *  Created on: 09-04-2013
 *      Author: _CORE7
 */

#ifndef POLYGON_H_
#define POLYGON_H_

#include <glm/glm.hpp>
#include <vector>
#include <Shape/Edge.h>
using namespace glm;
using namespace std;

namespace OGLPool {

class Polygon {
public:
	Polygon();
	virtual ~Polygon();

	void draw();

	unsigned int size();
protected:
	void addEdge( vec2 v0, vec2 v1 );
	void addEdge( Edge2 e );

private:
	vector< Edge2 > edges;
};

} /* namespace OGLPool */
#endif /* POLYGON_H_ */
