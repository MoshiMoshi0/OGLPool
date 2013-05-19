/*
 * ImpulseConstraintSolver.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef IMPULSECONSTRAINTSOLVER_H_
#define IMPULSECONSTRAINTSOLVER_H_

#include <vector>
#include <glm/glm.hpp>
#include "SolverConstraint.h"
#include <Physics/ManifoldPoint.h>
#include <RigidBody/RigidBody.h>
#include "SolverInfo.h"
using namespace glm;
using namespace std;

namespace OGLPool {
namespace Physics {

class ImpulseConstraintSolver {
public:
	ImpulseConstraintSolver();
	virtual ~ImpulseConstraintSolver();

	void solveGroup( const vector< RigidBody* >& bodies, const vector< ManifoldPoint* >& infos  );
	void solveGroupSetup( const vector< RigidBody* >& bodies, const vector< ManifoldPoint* >& infos );
	void solveGroupIterate( const vector< RigidBody* >& bodies, const vector< ManifoldPoint* >& infos );
	void solveGroupFinish( const vector< RigidBody* >& bodies );

	void solveSingleIteration( int iteration, const vector< RigidBody* >& bodies, const vector< ManifoldPoint* >& infos );
	void resolveSingleConstraintRowLowerLimit(SolverConstraint& c);
	void resolveSingleConstraintRowGeneric(SolverConstraint& c);
	void resolveSplitPenetrationImpulseCacheFriendly( SolverConstraint& c );
	void setFrictionConstraintImpulse( SolverConstraint& constraint, SolverBody* sb0, SolverBody* sb1, ManifoldPoint* info );

	void setupContactConstraint( SolverConstraint& constraint, SolverBody* sb0, SolverBody* sb1, const vec3& r0, const vec3& r1, const vec3& n, ManifoldPoint* info, float& relaxation, vec3& vel, float& rel_vel );
	void addFrictionConstraint( SolverBody* sb0, SolverBody* sb1, const vec3& r0, const vec3& r1, const vec3& n, ManifoldPoint* info, uint frictionIndex, float& relaxation );
	void addRollingFrictionConstraint( SolverBody* sb0, SolverBody* sb1, const vec3& r0, const vec3& r1, const vec3& n, ManifoldPoint* info, uint frictionIndex, float& relaxation );

	SolverBody* getSolverBody( RigidBody* e );
	SolverBody* initSolverBody( RigidBody* e );
	void convertContact( ManifoldPoint* info );

	vector< SolverBody* > solverBodyPool;

	vector< SolverConstraint > frictionConstraintPool;
	vector< SolverConstraint > contactConstraintPool;
	vector< SolverConstraint > rollingFrictionConstraintPool;

	SolverInfo infoGlobal;
};

}
} /* namespace OGLPool */
#endif /* IMPULSECONSTRAINTSOLVER_H_ */
