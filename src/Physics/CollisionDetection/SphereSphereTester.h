/*
 * SphereSphereTester.h
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#ifndef SPHERESPHERETESTER_H_
#define SPHERESPHERETESTER_H_

#include "CollisionTester.h"
#include <RigidBody/Sphere.h>

namespace OGLPool {

class SphereSphereTester : public CollisionTester {
public:
	SphereSphereTester();
	SphereSphereTester( Sphere* e0, Sphere* e1 );
	virtual ~SphereSphereTester();

	bool overlapTest( ManifoldPoint* info );
	bool sweepTest( ManifoldPoint* info );

	void setBodies( Sphere* s0, Sphere* s1 );

	Sphere* s0;
	Sphere* s1;
};

} /* namespace OGLPool */
#endif /* SPHERESPHERETESTER_H_ */
