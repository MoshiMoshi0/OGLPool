/*
 * SphereMeshTester.cpp
 *
 *  Created on: 23-05-2013
 *      Author: _CORE7
 */

#include "SphereMeshTester.h"
#include "SphereTriangleTester.h"
#include <algorithm>
#include <iostream>
using namespace std;

namespace OGLPool {

SphereMeshTester::SphereMeshTester(){}
SphereMeshTester::SphereMeshTester( Sphere* s, Mesh* m ){ setBodies( s, m ); }

SphereMeshTester::~SphereMeshTester(){}

bool SphereMeshTester::broadphase(){
	BoundingBox sb = s->getBoundingBox();
	BoundingBox mb = m->getBoundingBox();

	if( !BoundingBox::intersects( sb, mb ) ) return false;

	triangleCachePool.clear();
	for( auto& triangle : m->triangles ){
		SphereTriangleTester tester( s, &triangle );
		if( tester.broadphase() ){
			triangleCachePool.push_back( triangle );
		}
	}

	return triangleCachePool.size() > 0;
}

bool SphereMeshTester::narrowphase( ContactManifold* info ){
	bool collided = false;
	for( auto& triangle : triangleCachePool ){
		SphereTriangleTester tester( s, &triangle );

		if( tester.narrowphase( info ))
			collided = true;
	}
	return collided;
}

void SphereMeshTester::setBodies( Sphere* s, Mesh* m ){ this->s = s; this->m = m; CollisionTester::setBodies( s, m ); }

} /* namespace OGLPool */
