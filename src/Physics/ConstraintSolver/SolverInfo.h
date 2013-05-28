/*
 * ContactSolverInfo.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef CONTACTSOLVERINFO_H_
#define CONTACTSOLVERINFO_H_

#include <App/Time.h>

namespace OGLPool {
namespace Physics {

enum SolverMode {
	SOLVER_RANDMIZE_ORDER = 1 << 0,
	SOLVER_USE_WARMSTARTING = 1 << 1,
	SOLVER_USE_2_FRICTION_DIRECTIONS = 1 << 2,
	SOLVER_ENABLE_FRICTION_DIRECTION_CACHING = 1 << 3,
	SOLVER_DISABLE_VELOCITY_DEPENDENT_FRICTION_DIRECTION = 1 << 4,
};

struct SolverInfo {
	float timeStep;
	int iterations;
	float erp;
	float erp2;

	bool splitImpulse;
	float splitImpulsePenetrationThreshold;
	float splitImpulseTurnErp;

	float linearSlop;
	float warmstartingFactor;

	int	solverMode;
	float singleAxisRollingFrictionThreshold;

	SolverInfo(){
		timeStep = Time::DELTA_TIME;
		iterations = 10;
		erp = 0.2f;
		erp2 = 0.8f;
		splitImpulse = true;
		splitImpulsePenetrationThreshold = -0.04f;
		splitImpulseTurnErp = 0.1f;
		linearSlop = 0.0f;
		warmstartingFactor = 0.85f;
		solverMode = SOLVER_RANDMIZE_ORDER | SOLVER_USE_WARMSTARTING | SOLVER_USE_2_FRICTION_DIRECTIONS | SOLVER_ENABLE_FRICTION_DIRECTION_CACHING;
		singleAxisRollingFrictionThreshold = 1e30f;
	}
};

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CONTACTSOLVERINFO_H_ */
