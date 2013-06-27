/*
 * SolverBody.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef SOLVERBODY_H_
#define SOLVERBODY_H_

#include <RigidBody/RigidBody.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <RigidBody/Integration/Euler.h>

#include <iostream>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace glm;

namespace OGLPool {
namespace Physics {

struct SolverBody {
	SolverBody(){}
	SolverBody( RigidBody* e ) {
		originalBody = e;
		linVel = e->linVel;
		angVel = e->angVel;
		pos = e->pos;
		rot = e->rot;
		massInv = e->massInv;
	}

	RigidBody* originalBody;

	vec3 deltaLinearVelocity;
	vec3 deltaAngularVelocity;
	vec3 pushVelocity;
	vec3 turnVelocity;

	vec3 linVel;
	vec3 angVel;
	vec3 pos;
	quat rot;

	float massInv;

	void internalApplyImpulse(const vec3& linearComponent, const vec3& angularComponent, float impulse){
		if( originalBody ){
			deltaLinearVelocity += linearComponent * impulse;
			deltaAngularVelocity += angularComponent * impulse;
		}
	}

	void internalApplyPushImpulse(const vec3& linearComponent, const vec3& angularComponent, float impulse) {
		if( originalBody ){
			pushVelocity += linearComponent * impulse;
			turnVelocity += angularComponent* impulse;
		}
	}

	void writebackVelocity() {
		if( originalBody ){
			linVel += deltaLinearVelocity;
			angVel += deltaAngularVelocity;
		}
	}

	void writebackVelocityAndTransform(float timeStep, float splitImpulseTurnErp){
		if( originalBody ){
			writebackVelocity();
			Euler::integrate( pushVelocity, turnVelocity*splitImpulseTurnErp, pos, rot, timeStep );
		}
	}
};

}
} /* namespace OGLPool */
#endif /* SOLVERBODY_H_ */
