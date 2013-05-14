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
	vec3 dp = s1->pos - s0->pos;
	float r = s0->radius + s1->radius;
	if( dot( dp, dp) <= r*r ){
		vec3 n = normalize( dp );
		info->point0 = s0->pos + n * s0->radius;
		info->point1 = s1->pos - n * s1->radius;
		info->depth = r - length( dp );
		info->normal = n;
		info->setOverlapping( true );
		return true;
	}
	return false;
}

bool sphereSphereSwept( Sphere* s0, Sphere* s1, ContactInfo* info ){
	vec3 dir = (s1->vel - s0->vel) * info->deltaTime;
	return sphereRayTest( s1->pos, dir, s0->pos, s0->radius + s1->radius, info );
}

bool sphereSphereTest( Sphere* s0, Sphere* s1, ContactInfo* info ){
	info->e0 = (Entity*) s0; info->e1 = (Entity*) s1;
	if( sphereSphereOverlap( s0, s1, info ) ) return true;
	return sphereSphereSwept( s0, s1, info );
}

} /* namespace Physics */
} /* namespace OGLPool */
