/*
 * ImpulseConstraint.cpp
 *
 *  Created on: 28-03-2013
 *      Author: _CORE7
 */

#include "ImpulseConstraint.h"

namespace OGLPool {
namespace Physics {

ImpulseConstraint::ImpulseConstraint( RigidBody* e0, RigidBody* e1 ) : Constraint(e0, e1) {
}

ImpulseConstraint::~ImpulseConstraint() {
}

} /* namespace Physics */
} /* namespace OGLPool */
