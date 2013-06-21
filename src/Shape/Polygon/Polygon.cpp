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
void Polygon<T>::clear(){
	points.clear();
	edges.clear();
}

template< class T >
void Polygon<T>::addPoint( T point ){
	points.push_back( point );
}

template< class T >
void Polygon<T>::addEdge( Edge<T> edge ){
	edges.push_back( edge );
}

template< class T >
vector< T >& Polygon<T>::getPoints(){
	return points;
}

template< class T >
const vector< T >& Polygon<T>::getPoints() const {
	return points;
}

template< class T >
vector< Edge<T> >& Polygon<T>::getEdges(){
	return edges;
}

template< class T >
const vector< Edge<T> >& Polygon<T>::getEdges() const {
	return edges;
}

template class Polygon< vec2 >;
template class Polygon< vec3 >;

} /* namespace OGLPool */
