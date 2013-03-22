/*
 * crImpulse.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef CRIMPULSE_H_
#define CRIMPULSE_H_

#include <Physics/ContactInfo.h>
#include <Entity/Entity.h>
#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {
namespace Physics {

void applyContactImpulse( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3& dv, vec3 n, float e );
void applyFrictionImpulse( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3& dv, vec3 n, float e );
void applyRollingFrictionImpulse( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3& da, vec3 n, float e );
void response( Entity* e0, Entity* e1, ContactInfo* info );

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CRIMPULSE_H_ */
