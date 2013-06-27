/*
 * SphereMeshTester.h
 *
 *  Created on: 23-05-2013
 *      Author: _CORE7
 */

#ifndef SPHEREMESHTESTER_H_
#define SPHEREMESHTESTER_H_

#include "CollisionTester.h"
#include <RigidBody/Sphere.h>
#include <RigidBody/Mesh.h>
#include "SphereTriangleTester.h"

namespace OGLPool {

class SphereMeshTester : public CollisionTester {
public:
	SphereMeshTester();
	SphereMeshTester( Sphere* s, Mesh* m );
	virtual ~SphereMeshTester();

	bool broadphase();
	bool narrowphase( ContactManifold* manifold );

	void setBodies( Sphere* s, Mesh* m );

	Sphere* s;
	Mesh* m;

	vector< Triangle3 > triangleCachePool;
};

} /* namespace OGLPool */
#endif /* SPHEREMESHTESTER_H_ */
