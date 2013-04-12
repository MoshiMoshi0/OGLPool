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

	void addPoint( vec2 point );
	void addEdge( Edge2 edge );
protected:
	vector< vec2 > points;
	vector< Edge2 > edges;
};

} /* namespace OGLPool */
#endif /* POLYGON_H_ */
