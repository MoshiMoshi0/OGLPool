/*
 * Constraint.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <RigidBody/RigidBody.h>

namespace OGLPool {
namespace Physics {

class Constraint {
public:
	Constraint( RigidBody* e0, RigidBody* e1 );
	virtual ~Constraint();

	float appliedImpulse;

	RigidBody* e0;
	RigidBody* e1;
};

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CONSTRAINT_H_ */
