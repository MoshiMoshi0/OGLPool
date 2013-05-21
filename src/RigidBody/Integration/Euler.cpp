/*
 * Euler.cpp
 *
 *  Created on: 21-03-2013
 *      Author: _CORE7
 */

#include "Euler.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

using namespace glm;
namespace OGLPool {

void Euler::integrate( RigidBody* e, float dt ){
	e->angVel += e->inertiaInv * e->torque * dt;
	e->rot = normalize( e->rot + 0.5f * quat(0,e->angVel * dt) * e->rot );

	e->linVel += e->force * e->massInv * dt;
	e->pos += e->linVel * dt;
}

void Euler::integrate( const vec3& vel, const vec3& angVel, vec3& pos, quat& rot, float dt ){
	pos += vel * dt;
	rot = normalize( rot + 0.5f * quat(0,angVel * dt) * rot );
}

} /* namespace OGLPool */
