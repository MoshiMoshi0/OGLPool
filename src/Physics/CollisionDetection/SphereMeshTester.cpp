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
	BoundingBox3 sb = s->getBoundingBox();
	BoundingBox3 mb = m->getBoundingBox();

	if( !BoundingBox3::intersects( sb, mb ) ) return false;

	triangleCachePool.clear();
	for( auto& triangle : m->triangles ){
		SphereTriangleTester tester( s, &triangle );
		if( tester.broadphase() ){
			triangleCachePool.push_back( triangle );
		}
	}

	return triangleCachePool.size() > 0;
}

bool SphereMeshTester::narrowphase( ContactManifold* manifold ){
	bool collided = false;
	manifold->setBodies( s, m );
	for( auto& triangle : triangleCachePool ){
		SphereTriangleTester tester( s, &triangle );

		if( tester.narrowphase( manifold ))
			collided = true;
	}

	return collided;
}

void SphereMeshTester::setBodies( Sphere* s, Mesh* m ){ this->s = s; this->m = m; CollisionTester::setBodies( s, m ); }

} /* namespace OGLPool */
