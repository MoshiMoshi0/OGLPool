/*
 * Polygon.cpp
 *
 *  Created on: 09-04-2013
 *      Author: _CORE7
 */

#include "Polygon.h"
#include <SFML/OpenGL.hpp>

namespace OGLPool {

Polygon::Polygon() {}

Polygon::~Polygon() {}

void Polygon::draw(){
	glBegin( GL_LINES );
		for( unsigned int i = 0; i < edges.size(); i++ ){
			glVertex3f( edges[i][0].x, 0, edges[i][0].y );
			glVertex3f( edges[i][1].x, 0, edges[i][1].y );
		}
	glEnd();
}

unsigned int Polygon::size(){
	return edges.size();
}

void Polygon::addEdge( vec2 v0, vec2 v1 ){
	edges.push_back( Edge2( v0, v1 ) );
}

void Polygon::addEdge( Edge2 e ){
	edges.push_back( e );
}

} /* namespace OGLPool */
