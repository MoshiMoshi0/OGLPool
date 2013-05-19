/*
 * ContactSolverInfo.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef CONTACTSOLVERINFO_H_
#define CONTACTSOLVERINFO_H_

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
	//float m_tau;
	//float m_damping;
	//float m_friction;
	float m_timeStep;
	//float m_restitution;
	int m_numIterations;
	//float m_maxErrorReduction;
	//float m_sor;
	float m_erp;
	float m_erp2;
	//float m_globalCfm;
	int m_splitImpulse;
	float m_splitImpulsePenetrationThreshold;
	float m_splitImpulseTurnErp;
	float m_linearSlop;
	float m_warmstartingFactor;

	int	m_solverMode;
	float m_singleAxisRollingFrictionThreshold;

	SolverInfo(){
		//m_tau = 0.6f;
		//m_damping = 1.0f;
		//m_friction = 0.3f;
		m_timeStep = 1.f/60.f;
		//m_restitution = 0.0f;
		//m_maxErrorReduction = 20.0f;
		m_numIterations = 10;
		m_erp = 0.2f;
		m_erp2 = 0.8f;
		//m_globalCfm = 0.0f;
		//m_sor = 1.0f;
		m_splitImpulse = true;
		m_splitImpulsePenetrationThreshold = -0.04f;
		m_splitImpulseTurnErp = 0.1f;
		m_linearSlop = 0.0f;
		m_warmstartingFactor = 0.85f;
		m_solverMode = SOLVER_RANDMIZE_ORDER | SOLVER_USE_WARMSTARTING | SOLVER_USE_2_FRICTION_DIRECTIONS | SOLVER_ENABLE_FRICTION_DIRECTION_CACHING;
		m_singleAxisRollingFrictionThreshold = 1e30f;
	}
};

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CONTACTSOLVERINFO_H_ */
