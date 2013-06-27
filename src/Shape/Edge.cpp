/*
 * Edge.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "Edge.h"
#include <SFML/OpenGL.hpp>

namespace OGLPool {

template< class T >
Edge<T>::Edge() {}
template< class T >
Edge<T>::Edge( T v0, T v1 ) {
	vertices[ 0 ] = v0; vertices[ 1 ] = v1;
	direction = ( v1 - v0 ) / length( v1 - v0 );
	extent = length( v1 - v0 ) / 2.0f;
	center = (v1 + v0) / 2.0f;
}

template< class T >
Edge<T>::~Edge() {}

template<>
void Edge<vec2>::render() const {
	glBegin(GL_LINES);
		glVertex3f( vertices[0].x, 0, vertices[0].y );
		glVertex3f( vertices[1].x, 0, vertices[1].y );
	glEnd();
}

template<>
void Edge<vec3>::render() const {
	glBegin(GL_LINES);
		glVertex3f( vertices[0].x, vertices[0].y, vertices[0].z );
		glVertex3f( vertices[1].x, vertices[1].y, vertices[1].z );
	glEnd();
}

template< class T >
T Edge<T>::closestPoint( T point ){
    T ab = vertices[1] - vertices[0];
    float t = dot(point - vertices[0], ab) / dot( ab, ab );
    t = clamp(t, 0.f, 1.f);
    return vertices[0] + t*ab;
}

template< class T >
T& Edge<T>::operator[](int i){
	assert( i <= 1 );
	return vertices[ i ];
}

template< class T >
T& Edge<T>::at( int i ){
	assert( i <= 1 );
	return vertices[i];
}

template< class T >
const T& Edge<T>::at( int i ) const {
	assert( i <= 1 );
	return vertices[i];
}

template< class T >
T Edge<T>::getCenter() const {
	return center;
}

template< class T >
T Edge<T>::getDirection() const {
	return direction;
}

template< class T >
float Edge<T>::getExtent() const {
	return extent;
}

template class Edge< vec2 >;
template class Edge< vec3 >;

} /* namespace OGLPool */
