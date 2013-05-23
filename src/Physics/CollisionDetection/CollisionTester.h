/*
 * CollisionTester.h
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#ifndef COLLISIONTESTER_H_
#define COLLISIONTESTER_H_

#include <RigidBody/RigidBody.h>
#include <Physics/ManifoldPoint.h>

namespace OGLPool {

class CollisionTester {
public:
	CollisionTester();
	CollisionTester( RigidBody* e0, RigidBody* e1 );
	virtual ~CollisionTester();

	virtual bool overlapTest( ManifoldPoint* info );
	virtual bool sweepTest( ManifoldPoint* info );

	virtual bool broadphase();
	virtual bool narrowphase( ManifoldPoint* info );

	virtual void setBodies( RigidBody* e0, RigidBody* e1 );

	RigidBody* e0;
	RigidBody* e1;
};

} /* namespace OGLPool */
#endif /* COLLISIONTESTER_H_ */
