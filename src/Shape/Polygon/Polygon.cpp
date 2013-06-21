/*
 * Polygon.cpp
 *
 *  Created on: 09-04-2013
 *      Author: _CORE7
 */

#include "Polygon.h"
#include <SFML/OpenGL.hpp>

namespace OGLPool {

template< class T >
Polygon<T>::Polygon() {}

template< class T >
Polygon<T>::~Polygon() {}

template< class T >
void Polygon<T>::render(){
	for( auto& edge : edges ){
		edge.render();
	}
}

template< class T >
void Polygon<T>::addPoint( T point ){
	points.push_back( point );
}

template< class T >
void Polygon<T>::addEdge( Edge<T> edge ){
	edges.push_back( edge );
}

template class Polygon< vec2 >;
template class Polygon< vec3 >;

} /* namespace OGLPool */
