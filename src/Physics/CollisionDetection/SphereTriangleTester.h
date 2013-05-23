/*
 * SphereTriangleTester.h
 *
 *  Created on: 23-05-2013
 *      Author: _CORE7
 */

#ifndef SPHERETRIANGLETESTER_H_
#define SPHERETRIANGLETESTER_H_

#include "CollisionTester.h"
#include <RigidBody/Sphere.h>
#include <Shape/Triangle.h>

namespace OGLPool {

class SphereTriangleTester : public CollisionTester {
public:
	SphereTriangleTester();
	SphereTriangleTester( Sphere* s, Triangle3* t );
	virtual ~SphereTriangleTester();

	bool overlapTest( ManifoldPoint* info );
	bool sweepTest( ManifoldPoint* info );

	void setBodies( Sphere* s, Triangle3* t );

	Sphere* s;
	Triangle3* t;
};

} /* namespace OGLPool */
#endif /* SPHERETRIANGLETESTER_H_ */
