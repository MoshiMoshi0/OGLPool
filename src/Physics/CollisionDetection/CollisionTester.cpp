/*
 * CollisionTester.cpp
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#include "CollisionTester.h"

namespace OGLPool {

CollisionTester::CollisionTester(){}
CollisionTester::CollisionTester( RigidBody* e0, RigidBody* e1 ){ setBodies( e0, e1 ); }

CollisionTester::~CollisionTester(){}

bool CollisionTester::overlapTest( ContactManifold* manifold ){ assert( 0 ); return false; }
bool CollisionTester::sweptTest( ContactManifold* manifold ){ assert( 0 ); return false; }
void CollisionTester::setBodies( RigidBody* e0, RigidBody* e1 ){ this->e0 = e0; this->e1 = e1; }

bool CollisionTester::narrowphase( ContactManifold* manifold ){
	manifold->setBodies( e0, e1 );
	if( overlapTest( manifold ) ) return true;
	return false;
	//return sweptTest( manifold );
}

bool CollisionTester::broadphase(){
	if( (e0 && e0->mass == 0) && (e1 && e1->mass == 0) ) return false;

	const BoundingBox3& b0 = e0->getBoundingBox();
	const BoundingBox3& b1 = e1->getBoundingBox();

	return BoundingBox3::intersects( b0, b1 );
}

} /* namespace OGLPool */
