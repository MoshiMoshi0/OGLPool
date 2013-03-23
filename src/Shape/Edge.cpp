/*
 * Edge.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "Edge.h"

namespace OGLPool {

template< class T >
Edge<T>::Edge() {}
template< class T >
Edge<T>::Edge( T v0, T v1 ) {
	vertices[ 0 ] = v0; vertices[ 1 ] = v1;
}

template< class T >
Edge<T>::~Edge() {}

template< class T >
T& Edge<T>::operator[](int i){
	return vertices[ i % 2 ];
}

template class Edge< vec2 >;
template class Edge< vec3 >;

} /* namespace OGLPool */
