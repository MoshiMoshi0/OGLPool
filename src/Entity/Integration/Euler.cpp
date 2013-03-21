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

void Euler::integrate( Entity* e, float dt ){
	e->angVel += e->inertiaInv * e->torque * dt;
	e->rot = normalize( e->rot + 0.5f * quat(0,e->angVel * dt) * e->rot );

	e->vel += e->force * e->massInv * dt;
	e->pos += e->vel * dt;
}

} /* namespace OGLPool */
