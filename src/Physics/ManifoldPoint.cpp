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

	float restitution0 = e0 ? e0->restitutionCoef : 1;
	float restitution1 = e1 ? e1->restitutionCoef : 1;
	float friction0 = e0 ? e0->frictionCoef : 1;
	float friction1 = e1 ? e1->frictionCoef : 1;
	float rolling0 = e0 ? e0->rollingFrictionCoef : 1;
	float rolling1 = e1 ? e1->rollingFrictionCoef : 1;

	combinedRestitution = restitution0 * restitution1;
	combinedRollingFriction = friction0 * friction1;
	combinedFriction = rolling0 * rolling1;
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
