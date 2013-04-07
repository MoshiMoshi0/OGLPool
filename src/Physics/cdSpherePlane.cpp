/*
 * cdSpherePlane.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "cdSpherePlane.h"

namespace OGLPool {
namespace Physics {

bool spherePlaneIntersection(){
	return false;
}

bool spherePlaneSwept( Sphere* s, Plane* p, ContactInfo* info ){
	vec3 vel = s->getVel() * info->deltaTime;
	float dist = dot( p->normal, s->getPos() ) - dot( p->normal, p->pos );
	float denom = dot( p->normal, vel );
	if( denom * dist >= 0.0f ){
		return false;
	}

	float r = dist > 0.0f ? s->radius : -s->radius;
	float t = ( r - dist ) / denom;
	if( t > 1 || t < 0 ) return false;

	vec3 point = s->getPos() + t * vel - r * p->normal;

	info->time = t;
	info->point0 = point;
	info->point1 = point;
	info->normal = p->normal;
	info->setColliding( true );
	return true;
}

bool spherePlaneOverlap( Sphere* s, Plane* p, ContactInfo* info ){
	float dist = dot( p->normal, s->getPos() ) - dot( p->normal, p->pos );
	if( fabs( dist ) <= s->radius ){
		float penetration = s->radius - fabs( dist );
		info->time = 0;
		info->point0 = s->getPos() - p->normal * s->radius;
		info->point1 = s->getPos() - p->normal * (s->radius - penetration);
		info->depth = penetration;
		info->normal = p->normal;
		info->setOverlapping( true );
		return true;
	}

	return false;
}

bool spherePlaneTest( Sphere* s, Plane* p, ContactInfo* info ){
	if( spherePlaneOverlap( s, p, info ) ) return true;
	return spherePlaneSwept( s, p, info );
}

} /* namespace Physics */
} /* namespace OGLPool */
