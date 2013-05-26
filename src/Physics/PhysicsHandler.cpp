/*
 * PhysicsHandler.cpp
 *
 *  Created on: 16-05-2013
 *      Author: _CORE7
 */

#include "PhysicsHandler.h"
#include <iostream>
#include <algorithm>
using namespace std;

namespace OGLPool {
namespace Physics {

int getHash( int a, int b ){
	if( b < a ) swap( a, b );
	return (a + b)*(a + b + 1)/2 + b;
}

PhysicsHandler::PhysicsHandler(){}

PhysicsHandler::~PhysicsHandler(){
	for( auto& pair : persistentManifolds ){
		delete pair.second;
	}
	persistentManifolds.clear();
}

void PhysicsHandler::processBodies( const vector< RigidBody* > bodies, float dt ){
	for( auto it0 = bodies.begin(); it0 != bodies.end() - 1; it0++ ){
		for( auto it1 = it0 + 1; it1 != bodies.end(); it1++ ){
			RigidBody* e0 = (*it0);
			RigidBody* e1 = (*it1);

			CollisionTester* tester = getCollisionTester( e0, e1 );
			if( !tester ) continue;
			if( tester->broadphase() ){
				ContactManifold* manifold = getManifold( e0, e1 );

				bool newInfo = (manifold == 0);
				if( newInfo ){
					manifold = new ContactManifold();
				}else{
					//manifold->reset();
				}

				if( tester->narrowphase( manifold ) ){
					if( newInfo ){
						addManifold( e0, e1, manifold );
					}
					solverManifolds.push_back( manifold );
				} else if( newInfo ) {
					delete manifold;
				}
			}else{
				removeManifold( e0, e1 );
			}
			delete tester;
		}
	}

	solveCollisions( bodies, dt );
}

bool PhysicsHandler::removeManifold( const RigidBody* e0, const RigidBody* e1 ){
	int hash = getHash( e0->id, e1->id );
	auto it = persistentManifolds.find( hash );

	if( it == persistentManifolds.end() ) return false;
	delete it->second;
	persistentManifolds.erase( it );
	return true;
}

void PhysicsHandler::addManifold( const RigidBody* e0, const RigidBody* e1, ContactManifold* info ){
	int hash = getHash( e0->id, e1->id );
	persistentManifolds.insert( pair<int,ContactManifold*>( hash, info ) );
}

ContactManifold* PhysicsHandler::getManifold( const RigidBody* e0, const RigidBody* e1 ){
	int hash = getHash( e0->id, e1->id );
	auto it = persistentManifolds.find( hash );
	if( it == persistentManifolds.end() ) return 0;
	return it->second;
}

void PhysicsHandler::solveCollisions( const vector< RigidBody* > bodies, float dt ){
	if( solverManifolds.size() == 0 ) return;

	/*sort(infos.begin(), infos.end(),
		[](ManifoldPoint* i0, ManifoldPoint* i1) -> bool {
			return i0->time < i1->time;
		}
	);*/

	solver.solveGroup( bodies, solverManifolds, dt );
	solverManifolds.clear();
}

CollisionTester* PhysicsHandler::getCollisionTester( RigidBody* e0, RigidBody* e1 ){
	bool e0Sphere = e0->getType() == RigidBody::SPHERE;
	bool e0Plane = e0->getType() == RigidBody::PLANE;
	bool e0Mesh = e0->getType() == RigidBody::MESH;
	bool e1Sphere = e1->getType() == RigidBody::SPHERE;
	bool e1Plane = e1->getType() == RigidBody::PLANE;
	bool e1Mesh = e1->getType() == RigidBody::MESH;

	if( e0Sphere ) {
		if(e1Sphere) return new SphereSphereTester((Sphere*) e0, (Sphere*) e1);
		if(e1Plane) return new SpherePlaneTester((Sphere*) e0, (Plane*) e1);
		if(e1Mesh) return new SphereMeshTester((Sphere*) e0, (Mesh*) e1);
	}else if (e0Plane) {
		if (e1Sphere) return new SpherePlaneTester((Sphere*) e1, (Plane*) e0);
	}else if( e0Mesh ) {
		if( e1Sphere ) return new SphereMeshTester((Sphere*) e1, (Mesh*) e0);
	}

	return 0;
}

}
} /* namespace OGLPool */
