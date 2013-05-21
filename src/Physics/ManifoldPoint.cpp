/*
 * ContactInfo.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "ManifoldPoint.h"

namespace OGLPool {

ManifoldPoint::ManifoldPoint( float deltaTime ) : deltaTime( deltaTime ) {
	reset();

	lateralFrictionDir1 = vec3();
	lateralFrictionDir2 = vec3();

	combinedFriction = 0.0f;
	combinedRollingFriction = 0.0f;
	combinedRestitution = 0.0f;

	lateralFrictionInitialized = false;
	appliedImpulse = 0.0f;
	appliedImpulseLateral1 = 0.0f;
	appliedImpulseLateral2 = 0.0f;
}

ManifoldPoint::~ManifoldPoint() {}

void ManifoldPoint::setBodies( RigidBody* e0, RigidBody* e1 ){
	this->e0 = e0; this->e1 = e1;
	combinedRestitution = e0->restitutionCoef * e1->restitutionCoef;
	combinedRollingFriction = e0->frictionCoef * e1->frictionCoef;
	combinedFriction = e0->rollingFrictionCoef * e1->rollingFrictionCoef;
}

void ManifoldPoint::reset(){
	point0 = vec3();
	point1 = vec3();
	normal = vec3();

	e0 = e1 = 0;
	time = 0;
	depth = 0;
}

} /* namespace OGLPool */
