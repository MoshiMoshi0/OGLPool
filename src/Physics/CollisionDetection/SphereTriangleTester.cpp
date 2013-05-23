/*
 * SphereTriangleTester.cpp
 *
 *  Created on: 23-05-2013
 *      Author: _CORE7
 */

#include "SphereTriangleTester.h"

namespace OGLPool {

SphereTriangleTester::SphereTriangleTester(){}
SphereTriangleTester::SphereTriangleTester( Sphere* s, Triangle3* t ){ setBodies( s, t ); }

SphereTriangleTester::~SphereTriangleTester(){}

bool SphereTriangleTester::overlapTest( ManifoldPoint* info ){
	return false;
}
bool SphereTriangleTester::sweepTest( ManifoldPoint* info ){
	return false;
}

void SphereTriangleTester::setBodies( Sphere* s, Triangle3* t ){ this->s = s; this->t = t; CollisionTester::setBodies( s, 0 ); }

} /* namespace OGLPool */
