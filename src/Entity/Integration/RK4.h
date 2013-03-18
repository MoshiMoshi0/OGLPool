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
#include <Entity/Entity.h>

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
	static void integrate( Entity* e, float dt );
private:
	static Derivative eval(Entity* e, float dt );
	static Derivative eval( Entity* e, float dt, Derivative derivative );
	static vec3 linAcc( Entity* e );
	static vec3 angAcc( Entity* e );
};

} /* namespace OGLPool */
#endif /* RK4_H_ */
