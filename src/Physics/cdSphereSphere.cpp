/*
 * cdSphereSphere.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "cdSphereSphere.h"
#include "cdSphereRay.h"

namespace OGLPool {
namespace Physics {

bool sphereSphereIntersection( Sphere* s0, Sphere* s1 ){
	return length(s0->pos + s1->pos) <= s0->radius + s1->radius;
}

bool sphereSphereOverlap( Sphere* s0, Sphere* s1, ContactInfo* info ){
	return false;
}

bool sphereSphereSwept( Sphere* s0, Sphere* s1, ContactInfo* info ){
	vec3 dp = s1->pos - s0->pos;
	float r = s0->radius + s1->radius;
	if( dot( dp, dp) <= r*r ){
		info->setOverlapping( true );
		info->setDepth( r - length( dp ) );
		info->setNormal( normalize( dp ) );
		return true;
	}
	vec3 dir = s1->vel - s0->vel;
	return sphereRayTest( s1->pos, dir, s0->pos, s0->radius + s1->radius, info );
}

bool sphereSphereTest( Sphere* s0, Sphere* s1, ContactInfo* info ){
	if( sphereSphereOverlap( s0, s1, info ) ) return true;
	return sphereSphereSwept( s0, s1, info );
}

} /* namespace Physics */
} /* namespace OGLPool */
