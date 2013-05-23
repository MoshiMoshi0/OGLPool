/*
 * SphereMeshTester.cpp
 *
 *  Created on: 23-05-2013
 *      Author: _CORE7
 */

#include "SphereMeshTester.h"
#include "SphereTriangleTester.h"

namespace OGLPool {

SphereMeshTester::SphereMeshTester(){}
SphereMeshTester::SphereMeshTester( Sphere* s, Mesh* m ){ setBodies( s, m ); }

SphereMeshTester::~SphereMeshTester(){}

bool SphereMeshTester::broadphase(){
	BoundingBox sb = s->getBoundingBox();
	BoundingBox mb = m->getBoundingBox();

	if( !BoundingBox::intersects( sb, mb ) ) return false;

	vector< Triangle3 >* triangles = m->getTriangles();
	for( auto& triangle : *triangles ){
		BoundingBox tb = BoundingBox::get( {triangle[0],triangle[1],triangle[2]}, 0.1f );
		if( BoundingBox::intersects( sb, tb ) ){
			triangleCachePool.push_back( triangle );
		}
	}

	return triangleCachePool.size() > 0;
}

bool SphereMeshTester::narrowphase( ManifoldPoint* info ){
	SphereTriangleTester tester;
	for( uint i = 0; i < triangleCachePool.size(); i++ ){
		Triangle3 t = triangleCachePool[i];

		tester.setBodies( s, &t );
	}
	return false;
}

void SphereMeshTester::setBodies( Sphere* s, Mesh* m ){ this->s = s; this->m = m; CollisionTester::setBodies( s, m ); }

} /* namespace OGLPool */
