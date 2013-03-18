/*
 * Sphere.cpp
 *
 *  Created on: 18-03-2013
 *      Author: _CORE7
 */

#include "Sphere.h"
#include <Entity/Integration/RK4.h>
#include <glm/gtx/vector_access.hpp>

using namespace glm;
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
	if( massInv > 0 ){
		RK4::integrate( this, dt );
		set( force, .0f,.0f,.0f );
		set( torque, .0f,.0f,.0f );
	}
}

void Sphere::render(){
	glColor3f( 0.5,0.2,0.5 );
	glPushMatrix();
		applyTransform();
		gluSphere( quadric , radius , gluSlices , gluStacks );
	glPopMatrix();
}
} /* namespace OGLPool */
