/*
 * CollisionTester.h
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#ifndef COLLISIONTESTER_H_
#define COLLISIONTESTER_H_

#include <RigidBody/RigidBody.h>
#include <Physics/ContactManifold.h>

namespace OGLPool {

class CollisionTester {
public:
	CollisionTester();
	CollisionTester( RigidBody* e0, RigidBody* e1 );
	virtual ~CollisionTester();

	virtual bool overlapTest( ContactManifold* manifold );
	virtual bool sweptTest( ContactManifold* manifold );

	virtual bool broadphase();
	virtual bool narrowphase( ContactManifold* manifold );

	virtual void setBodies( RigidBody* e0, RigidBody* e1 );

	RigidBody* e0;
	RigidBody* e1;
};

} /* namespace OGLPool */
#endif /* COLLISIONTESTER_H_ */
