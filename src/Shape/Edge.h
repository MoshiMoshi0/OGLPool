/*
 * Edge.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef EDGE_H_
#define EDGE_H_

#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {

template< class T >
class Edge {
public:
	Edge();
	Edge( T v0, T v1 );
	virtual ~Edge();

	T& operator[](int i);
private:
	T vertices[ 2 ];
};

typedef Edge< vec2 > Edge2;
typedef Edge< vec3 > Edge3;

} /* namespace OGLPool */
#endif /* EDGE_H_ */
