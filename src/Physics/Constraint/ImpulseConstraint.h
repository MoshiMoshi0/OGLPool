/*
 * ImpulseConstraint.h
 *
 *  Created on: 28-03-2013
 *      Author: _CORE7
 */

#ifndef IMPULSECONSTRAINT_H_
#define IMPULSECONSTRAINT_H_

#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {
namespace Physics {

class ImpulseConstraint {
public:
	ImpulseConstraint();
	virtual ~ImpulseConstraint();

	vec3 impulse;
};

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* IMPULSECONSTRAINT_H_ */
