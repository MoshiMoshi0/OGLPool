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
#include <Physics/CollisionDetection/CollisionDetection.h>
#include <Physics/ConstraintSolver/ImpulseConstraintSolver.h>
using namespace std;

namespace OGLPool {
namespace Physics {

class PhysicsHandler {
public:
	PhysicsHandler();
	virtual ~PhysicsHandler();

	void processBodies( const vector< RigidBody* > bodies, float dt );
	void solveCollisions( const vector< RigidBody* > bodies, float dt );

	bool removeManifold( const RigidBody* e0, const RigidBody* e1 );
	ManifoldPoint* getManifold( const RigidBody* e0, const RigidBody* e1 );
	void addManifold( const RigidBody* e0, const RigidBody* e1, ManifoldPoint* info );
	void addManifold( ManifoldPoint* info );

	bool processBodyPair( RigidBody* e0, RigidBody* e1 );

	CollisionTester* getCollisionTester( RigidBody* e0, RigidBody* e1 );

private:
	map< int, ManifoldPoint* > persistentManifolds;
	vector< ManifoldPoint* > solverManifolds;
	ImpulseConstraintSolver solver;
	SolverInfo infoGlobal;
};

}
} /* namespace OGLPool */
#endif /* PHYSICSHANDLER_H_ */
