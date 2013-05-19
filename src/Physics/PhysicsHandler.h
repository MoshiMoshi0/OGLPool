/*
 * PhysicsHandler.h
 *
 *  Created on: 16-05-2013
 *      Author: _CORE7
 */

#ifndef PHYSICSHANDLER_H_
#define PHYSICSHANDLER_H_

#include <map>
#include <vector>
#include <RigidBody/RigidBody.h>
#include <Physics/ManifoldPoint.h>
#include <Physics/ConstraintSolver/ImpulseConstraintSolver.h>
using namespace std;

namespace OGLPool {
namespace Physics {

class PhysicsHandler {
public:
	PhysicsHandler();
	virtual ~PhysicsHandler();

	void processBodies( const vector< RigidBody* > bodies, float dt );
	void solveCollisions( const vector< RigidBody* > bodies );

	bool removeManifold( const RigidBody* e0, const RigidBody* e1 );
	ManifoldPoint* getManifold( const RigidBody* e0, const RigidBody* e1 );
	void addManifold( const RigidBody* e0, const RigidBody* e1, ManifoldPoint* info );

	bool processBodyPair( RigidBody* e0, RigidBody* e1 );
	bool checkNarrowphase( RigidBody* e0, RigidBody* e1, ManifoldPoint* info );
	bool checkBroadphase( RigidBody* e0, RigidBody* e1 );

private:
	map< int, ManifoldPoint* > broadphaseInfos;
	vector< ManifoldPoint* > narrowphaseInfos;
	ImpulseConstraintSolver solver;
	SolverInfo infoGlobal;
};

}
} /* namespace OGLPool */
#endif /* PHYSICSHANDLER_H_ */
