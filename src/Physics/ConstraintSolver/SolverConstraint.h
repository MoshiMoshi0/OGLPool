/*
 * SolverConstraint.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef SOLVERCONSTRAINT_H_
#define SOLVERCONSTRAINT_H_

#include "SolverBody.h"
#include <Physics/ContactInfo.h>

namespace OGLPool {
namespace Physics {

struct SolverConstraint {
	SolverConstraint( SolverBody* sb0, SolverBody* sb1 ) : sb0( sb0 ), sb1( sb1 ){};

	SolverBody* sb0;
	SolverBody* sb1;

	vec3 m_relpos1CrossNormal;
	vec3 m_contactNormal;

	vec3 m_relpos2CrossNormal;

	vec3 m_angularComponentA;
	vec3 m_angularComponentB;

	float m_appliedPushImpulse;
	float m_appliedImpulse;

	float m_friction;
	float m_jacDiagABInv;
	float m_rhs;
	float m_cfm;

	float m_lowerLimit;
	float m_upperLimit;
	float m_rhsPenetration;

	uint m_frictionIndex;

	ContactInfo* m_originalContactPoint;
};

}
} /* namespace OGLPool */
#endif /* SOLVERCONSTRAINT_H_ */
