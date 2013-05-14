/*
 * Constraint.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef CONSTRAINT_H_
#define CONSTRAINT_H_

#include <Entity/Entity.h>

namespace OGLPool {
namespace Physics {

class Constraint {
public:
	Constraint( Entity* e0, Entity* e1 );
	virtual ~Constraint();

	float appliedImpulse;

	Entity* e0;
	Entity* e1;
};

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CONSTRAINT_H_ */
