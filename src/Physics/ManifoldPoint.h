/*
 * ContactInfo.h
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#ifndef CONTACTINFO_H_
#define CONTACTINFO_H_

#include <RigidBody/RigidBody.h>
#include <glm/glm.hpp>

using namespace glm;

namespace OGLPool {

class ManifoldPoint {
public:
	ManifoldPoint();
	virtual ~ManifoldPoint();

	void setBodies( RigidBody* e0, RigidBody* e1 );
	void reset();

	vec3 normal;
	vec3 point0;
	vec3 point1;
	vec3 local0;
	vec3 local1;

	float depth;
	float time;

	float combinedRollingFriction;
	float combinedFriction;
	float combinedRestitution;

	float appliedImpulse;
	float appliedImpulseLateral1;
	float appliedImpulseLateral2;
	bool lateralFrictionInitialized;
	vec3 lateralFrictionDir1;
	vec3 lateralFrictionDir2;
};

} /* namespace OGLPool */
#endif /* CONTACTINFO_H_ */
