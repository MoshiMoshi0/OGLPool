/*
 * Sphere.cpp
 *
 *  Created on: 18-03-2013
 *      Author: _CORE7
 */

#include "Sphere.h"

namespace OGLPool {

Sphere::Sphere( float radius ) : Entity() {
	this->radius = radius;
	this->gluSlices = 36;
	this->gluStacks = 18;

	float x = 2.0f / 5.0f * mass * radius * radius;
	setInertia( mat3(x,0,0,0,x,0,0,0,x) );

	quadric = gluNewQuadric();
	gluQuadricDrawStyle(quadric, GLU_LINE );
	gluQuadricNormals( quadric, GLU_FLAT );
}

Sphere::Sphere( float radius, vec3 pos ) : Sphere( radius ) {
	this->pos = pos;
}

Sphere::~Sphere() {
	gluDeleteQuadric(quadric);
}

void Sphere::update( float dt ){

}

void Sphere::render(){
	glPushMatrix();
		glTranslatef( pos.x, pos.y, pos.z );
		gluSphere( quadric , radius , gluSlices , gluStacks );
	glPopMatrix();
}
} /* namespace OGLPool */
