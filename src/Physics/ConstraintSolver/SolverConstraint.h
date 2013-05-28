/*
 * SolverConstraint.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef SOLVERCONSTRAINT_H_
#define SOLVERCONSTRAINT_H_

#include "SolverBody.h"
#include <Physics/ManifoldPoint.h>

namespace OGLPool {
namespace Physics {

struct SolverConstraint {
	SolverConstraint( SolverBody* sb0, SolverBody* sb1 ) : sb0( sb0 ), sb1( sb1 ){};

	SolverBody* sb0;
	SolverBody* sb1;

	vec3 r0CrossN;
	vec3 r1CrossN;

	vec3 contactNormal;

	vec3 angularComponent0;
	vec3 angularComponent1;

	float appliedPushImpulse;
	float appliedImpulse;

	float friction;
	float effectiveMass;
	float rhs;
	float cfm;

	float lowerLimit;
	float upperLimit;
	float rhsPenetration;

	uint frictionIndex;

	ManifoldPoint* originalPoint;
};

}
} /* namespace OGLPool */
#endif /* SOLVERCONSTRAINT_H_ */
