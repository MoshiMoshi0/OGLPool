/*
 * Triangle.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "Triangle.h"

namespace OGLPool {

template< class T >
Triangle<T>::Triangle() {}
template< class T >
Triangle<T>::Triangle( T v0, T v1, T v2 ) {
	vertices[ 0 ] = v0; vertices[ 1 ] = v1; vertices[ 2 ] = v2;
}

template< class T >
Triangle<T>::Triangle( Edge<T> e0, Edge<T> e1, Edge<T> e2 ) {
	vertices[ 0 ] = e0[0]; vertices[ 1 ] = e1[0]; vertices[ 2 ] = e2[0];
}

template< class T >
Triangle<T>::~Triangle() {}

template< class T >
Edge<T> Triangle<T>::getEdge( int i ){
	int i0 = i % 3;
	int i1 = (i0 + 1) % 3;

	return Edge<T>( vertices[i0], vertices[i1] );
}

template< class T >
T Triangle<T>::getNormal(){
	T v10 = vertices[1] - vertices[0];
	T v20 = vertices[2] - vertices[0];

	return normalize( cross( v10, v20 ) );
}

template< class T >
T& Triangle<T>::operator[](int i){
	return vertices[ i % 3 ];
}

} /* namespace OGLPool */
