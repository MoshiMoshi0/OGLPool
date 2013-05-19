/*
 * RK4.h
 *
 *  Created on: 18-03-2013
 *      Author: _CORE7
 */

#ifndef RK4_H_
#define RK4_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <RigidBody/RigidBody.h>

using namespace glm;

namespace OGLPool {

class Derivative{
public:
	vec3 vel;
	vec3 acc;

	quat spin;
	vec3 angAcc;

	Derivative(){}
	Derivative( vec3 vel, vec3 acc, quat spin, vec3 angAcc ): vel(vel), acc(acc), spin(spin), angAcc(angAcc) {}
};

class RK4 {
public:
	static void integrate( RigidBody* e, float dt );
private:
	static Derivative eval(RigidBody* e, float dt );
	static Derivative eval( RigidBody* e, float dt, Derivative derivative );
	static vec3 linAcc( RigidBody* e );
	static vec3 angAcc( RigidBody* e );
};

} /* namespace OGLPool */
#endif /* RK4_H_ */
