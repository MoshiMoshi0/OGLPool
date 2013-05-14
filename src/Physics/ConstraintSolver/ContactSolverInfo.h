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

enum btSolverMode {
	SOLVER_RANDMIZE_ORDER = 1,
	SOLVER_FRICTION_SEPARATE = 2,
	SOLVER_USE_WARMSTARTING = 4,
	SOLVER_USE_2_FRICTION_DIRECTIONS = 16,
	SOLVER_ENABLE_FRICTION_DIRECTION_CACHING = 32,
	SOLVER_DISABLE_VELOCITY_DEPENDENT_FRICTION_DIRECTION = 64,
	SOLVER_CACHE_FRIENDLY = 128,
	SOLVER_SIMD = 256,
	SOLVER_INTERLEAVE_CONTACT_AND_FRICTION_CONSTRAINTS = 512,
	SOLVER_ALLOW_ZERO_LENGTH_FRICTION_DIRECTIONS = 1024
};

struct ContactSolverInfoData {
	float	m_tau;
	float	m_damping;
	float	m_friction;
	float	m_timeStep;
	float	m_restitution;
	int		m_numIterations;
	float	m_maxErrorReduction;
	float	m_sor;
	float	m_erp;//used as Baumgarte factor
	float	m_erp2;//used in Split Impulse
	float	m_globalCfm;//constraint force mixing
	int			m_splitImpulse;
	float	m_splitImpulsePenetrationThreshold;
	float	m_splitImpulseTurnErp;
	float	m_linearSlop;
	float	m_warmstartingFactor;

	int			m_solverMode;
	int	m_restingContactRestitutionThreshold;
	int			m_minimumSolverBatchSize;
	float	m_maxGyroscopicForce;
	float	m_singleAxisRollingFrictionThreshold;
};

struct ContactSolverInfo : public ContactSolverInfoData {
	inline ContactSolverInfo(){
		m_tau = float(0.6);
		m_damping = float(1.0);
		m_friction = float(0.3);
		m_timeStep = float(1.f/60.f);
		m_restitution = float(0.);
		m_maxErrorReduction = float(20.);
		m_numIterations = 10;
		m_erp = float(0.2);
		m_erp2 = float(0.8);
		m_globalCfm = float(0.);
		m_sor = float(1.);
		m_splitImpulse = true;
		m_splitImpulsePenetrationThreshold = -.04f;
		m_splitImpulseTurnErp = 0.1f;
		m_linearSlop = float(0.0);
		m_warmstartingFactor=float(0.85);
		m_solverMode = SOLVER_USE_WARMSTARTING | SOLVER_SIMD;
		//m_maxGyroscopicForce = 100.f; ///only used to clamp forces for bodies that have their BT_ENABLE_GYROPSCOPIC_FORCE flag set (using btRigidBody::setFlag)
		m_singleAxisRollingFrictionThreshold = 1e30f;///if the velocity is above this threshold, it will use a single constraint row (axis), otherwise 3 rows.
	}
};

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CONTACTSOLVERINFO_H_ */
