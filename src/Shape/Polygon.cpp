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

void Polygon::addPoint( vec2 point ){
	points.push_back( point );
}

void void Polygon::addEdge( Edge2 edge ){
	edges.push_back( edge );
}

} /* namespace OGLPool */
