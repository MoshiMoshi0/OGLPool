/*
 * PhysicsHandler.cpp
 *
 *  Created on: 16-05-2013
 *      Author: _CORE7
 */

#include "PhysicsHandler.h"
#include <typeinfo>
#include <Physics/CollisionDetection/CollisionDetection.h>
#include <iostream>
#include <algorithm>
using namespace std;

namespace OGLPool {
namespace Physics {

PhysicsHandler::PhysicsHandler(){}

PhysicsHandler::~PhysicsHandler(){
	for( auto& pair : broadphaseInfos ){
		delete pair.second;
	}
	broadphaseInfos.clear();
}

void PhysicsHandler::processBodies( const vector< RigidBody* > bodies, float dt ){
	infoGlobal.m_timeStep = dt;
	for( auto it0 = bodies.begin(); it0 != bodies.end() - 1; it0++ ){
		for( auto it1 = it0 + 1; it1 != bodies.end(); it1++ ){
			RigidBody* e0 = (*it0);
			RigidBody* e1 = (*it1);

			if( checkBroadphase( e0, e1 ) ){
				ManifoldPoint* info = getManifold( e0, e1 );

				bool newInfo = (info == 0);
				if( newInfo ){
					info = new ManifoldPoint( dt );
				}else{
					info->reset();
				}

				if( checkNarrowphase( e0, e1, info ) ){
					if( newInfo ){
						addManifold( e0, e1, info );
					}
					narrowphaseInfos.push_back( info );
				} else if( newInfo ) {
					delete info;
				}
			}else{
				removeManifold( e0, e1 );
			}
		}
	}

	solveCollisions( bodies );
}

int getHash( int a, int b ){
	if( b < a ) swap( a, b );
	return (a + b)*(a + b + 1)/2 + b;
}

bool PhysicsHandler::removeManifold( const RigidBody* e0, const RigidBody* e1 ){
	int hash = getHash( e0->id, e1->id );
	auto it = broadphaseInfos.find( hash );

	if( it == broadphaseInfos.end() ) return false;
	delete it->second;
	broadphaseInfos.erase( it );
	return true;
}

void PhysicsHandler::addManifold( const RigidBody* e0, const RigidBody* e1, ManifoldPoint* info ){
	int hash = getHash( e0->id, e1->id );
	broadphaseInfos.insert( pair<int,ManifoldPoint*>( hash, info ) );
}

ManifoldPoint* PhysicsHandler::getManifold( const RigidBody* e0, const RigidBody* e1 ){
	int hash = getHash( e0->id, e1->id );
	auto it = broadphaseInfos.find( hash );
	if( it == broadphaseInfos.end() ) return 0;
	return it->second;
}

void PhysicsHandler::solveCollisions( const vector< RigidBody* > bodies ){
	if( narrowphaseInfos.size() == 0 ) return;

	/*sort(infos.begin(), infos.end(),
		[](ManifoldPoint* i0, ManifoldPoint* i1) -> bool {
			return i0->time < i1->time;
		}
	);*/

	solver.solveGroup( bodies, narrowphaseInfos, &infoGlobal );
	narrowphaseInfos.clear();
}

bool PhysicsHandler::checkNarrowphase( RigidBody* e0, RigidBody* e1, ManifoldPoint* info ){
	auto *i0 = &typeid (*e0);
	auto *i1 = &typeid (*e1);

	bool e0Sphere = *i0 == typeid(Sphere);
	bool e0Plane = *i0 == typeid(Plane);
	bool e1Sphere = *i1 == typeid(Sphere);
	bool e1Plane = *i1 == typeid(Plane);
	if(e0Sphere) {
		if(e1Sphere) return sphereSphereTest((Sphere*) e0, (Sphere*) e1, info);
		if(e1Plane) return spherePlaneTest((Sphere*) e0, (Plane*) e1, info);
	}else if (e0Plane) {
		if (e1Sphere) return spherePlaneTest((Sphere*) e1, (Plane*) e0, info );
	}

	return false;
}

bool PhysicsHandler::checkBroadphase( RigidBody* e0, RigidBody* e1 ){
	if( e0->mass == 0 && e1->mass == 0 ) return false;

	const BoundingBox& b0 = e0->getBoundingBox();
	const BoundingBox& b1 = e1->getBoundingBox();

	return BoundingBox::intersects( b0, b1 );
}

}
} /* namespace OGLPool */
