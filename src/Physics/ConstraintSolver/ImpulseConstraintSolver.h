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
#include <Physics/ContactInfo.h>
#include <Entity/Entity.h>
#include "ContactSolverInfo.h"
using namespace glm;
using namespace std;

namespace OGLPool {
namespace Physics {

class ImpulseConstraintSolver {
public:
	ImpulseConstraintSolver();
	virtual ~ImpulseConstraintSolver();

	void solveGroup( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos  );
	void solveGroupSetup( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos );
	void solveGroupIterate( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos );
	void solveGroupFinish( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos );

	void solveSingleIteration( int iteration, const vector< Entity* >& bodies, const vector< ContactInfo* >& infos );
	void resolveSingleConstraintRowLowerLimit(SolverConstraint& c);
	vector< SolverBody* > solverBodyPool;

	vector< SolverConstraint > frictionConstraintPool;
	vector< SolverConstraint > contactConstraintPool;
	vector< SolverConstraint > rollingFrictionConstraintPool;

	ContactSolverInfo infoGlobal;

	void addContactConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, float& relaxation, vec3& vel, float& rel_vel );
	void addFrictionConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, float& relaxation );
	void addRollingFrictionConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, float& relaxation );

	SolverBody* getSolverBody( Entity* e );
	void convertContact( ContactInfo* info );
};

}
} /* namespace OGLPool */
#endif /* IMPULSECONSTRAINTSOLVER_H_ */
