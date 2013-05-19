/*
 * Constraint.cpp
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#include "Constraint.h"

namespace OGLPool {
namespace Physics {

Constraint::Constraint( RigidBody* e0, RigidBody* e1 ) : e0(e0), e1(e1) {
	appliedImpulse = 0.0f;
}

Constraint::~Constraint(){
}

} /* namespace Physics */
} /* namespace OGLPool */
