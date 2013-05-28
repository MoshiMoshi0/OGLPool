/*
 * ContactInfo.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "ManifoldPoint.h"

namespace OGLPool {

ManifoldPoint::ManifoldPoint() {
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

void ManifoldPoint::reset(){
	point0 = vec3();
	point1 = vec3();
	local0 = vec3();
	local1 = vec3();
	normal = vec3();

	time = 0;
	depth = 0;
}

} /* namespace OGLPool */
