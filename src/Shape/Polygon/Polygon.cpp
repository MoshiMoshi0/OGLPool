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
		for( auto& edge : edges ){
			glVertex3f( edge[0].x, 0, edge[0].y );
			glVertex3f( edge[1].x, 0, edge[1].y );
		}
	glEnd();
}

void Polygon::addPoint( vec2 point ){
	points.push_back( point );
}

void Polygon::addEdge( Edge2 edge ){
	edges.push_back( edge );
}

vector< Edge2 >& Polygon::getEdges() {
	return edges;
}

} /* namespace OGLPool */
