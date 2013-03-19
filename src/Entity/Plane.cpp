/*
 * Plane.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "Plane.h"
#include <SFML/OpenGL.hpp>

namespace OGLPool {

Plane::Plane( vec3 normal, vec3 point ) : Entity( point ) {
	this->normal = normalize( normal );
	setInertia( mat3(0.0f) );
}

Plane::~Plane() {}

void Plane::render(){
	glColor3f( 1,1,1 );

	glPushMatrix();
	applyTransform();
	glBegin( GL_QUADS );
		glVertex3f( -20, 0, -20 );
		glVertex3f( 20, 0, -20 );
		glVertex3f( 20, 0, +20 );
		glVertex3f( -20, 0, +20 );
	glEnd();
	glPopMatrix();
}

} /* namespace OGLPool */
