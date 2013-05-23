/*
 * Sphere.cpp
 *
 *  Created on: 18-03-2013
 *      Author: _CORE7
 */

#include "Sphere.h"
#include <RigidBody/Integration/Euler.h>
#include <glm/gtx/vector_access.hpp>

using namespace glm;
namespace OGLPool {

Sphere::Sphere( float radius ) : RigidBody() {
	setMass( (4.0f/3.0f)*pi<float>()*powf(radius,3) * 5 );
	this->radius = radius;
	this->gluSlices = 36;
	this->gluStacks = 18;

	float x = 2.0f / 5.0f * mass * radius * radius;
	setInertia( mat3(x,0,0,0,x,0,0,0,x) );

	restitutionCoef = 0.6f;
	frictionCoef = 0.6f;
	rollingFrictionCoef = 0.6f;

	quadric = gluNewQuadric();
	gluQuadricDrawStyle( quadric, GLU_SMOOTH );
	gluQuadricNormals( quadric, GLU_FLAT );

	boundingBox = BoundingBox::get( vec3(1,1,1) * -radius, vec3(1,1,1) * radius, 0.1f );
}

Sphere::Sphere( float radius, vec3 pos ) : Sphere( radius ) {
	this->pos = pos;
}

Sphere::~Sphere() {
	gluDeleteQuadric(quadric);
}

void Sphere::update( float dt ){
	if( massInv > 0 ){
		Euler::integrate( this, dt );
		applyDamping( dt );
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
