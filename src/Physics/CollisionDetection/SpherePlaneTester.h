/*
 * SpherePlaneTester.h
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#ifndef SPHEREPLANETESTER_H_
#define SPHEREPLANETESTER_H_

#include "CollisionTester.h"
#include <RigidBody/Plane.h>
#include <RigidBody/Sphere.h>

namespace OGLPool {

class SpherePlaneTester : public CollisionTester {
public:
	SpherePlaneTester();
	SpherePlaneTester( Sphere* s, Plane* p );
	virtual ~SpherePlaneTester();

	bool overlapTest( ContactManifold* manifold );
	bool sweptTest( ContactManifold* manifold );

	void setBodies( Sphere* s, Plane* p );

	Sphere* s;
	Plane* p;
};

} /* namespace OGLPool */
#endif /* SPHEREPLANETESTER_H_ */
