/*
 * Euler.h
 *
 *  Created on: 21-03-2013
 *      Author: _CORE7
 */

#ifndef EULER_H_
#define EULER_H_

#include <RigidBody/RigidBody.h>

namespace OGLPool {

class Euler {
public:
	static void integrate( RigidBody* e, float dt );
	static void integrate( const vec3& vel, const vec3& angVel, vec3& pos, quat& rot, float dt );
};

} /* namespace OGLPool */
#endif /* EULER_H_ */
