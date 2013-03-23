/*
 * Triangle.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include <Shape/Edge.h>
#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {

template< class T >
class Triangle {
public:
	Triangle();
	Triangle( T v0, T v1, T v2 );
	Triangle( Edge<T> e0, Edge<T> e1, Edge<T> e2 );
	virtual ~Triangle();

	//@TODO getPlane();
	Edge<T> getEdge( int i );
	vec3 getNormal();
	T& operator[](int i);
private:
	T vertices[ 3 ];
};

typedef Triangle< vec2 > Triangle2;
typedef Triangle< vec3 > Triangle3;

} /* namespace OGLPool */
#endif /* TRIANGLE_H_ */
