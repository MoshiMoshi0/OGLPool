/*
 * Edge.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <glm/glm.hpp>
#include <Shape/Shape.h>
using namespace glm;

namespace OGLPool {

template< class T >
class Edge : public Shape {
public:
	Edge();
	Edge( T v0, T v1 );
	virtual ~Edge();

	T closestPoint( T point );
	T& operator[](int i);
	T getCenter();
	T getDirection();
	float getExtent();
private:
	T vertices[ 2 ];
	T center;
	float extent;
	T direction;
};

typedef Edge< vec2 > Edge2;
typedef Edge< vec3 > Edge3;

} /* namespace OGLPool */
#endif /* EDGE_H_ */
