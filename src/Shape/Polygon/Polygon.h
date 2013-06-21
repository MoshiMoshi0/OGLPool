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

template< class T >
class Polygon : public Shape {
public:
	Polygon();
	virtual ~Polygon();

	void render();

	void addPoint( T point );
	void addEdge( Edge<T> edge );
protected:
	vector< T > points;
	vector< Edge<T> > edges;
};

typedef Polygon< vec2 > Polygon2;
typedef Polygon< vec3 > Polygon3;

} /* namespace OGLPool */
#endif /* POLYGON_H_ */
