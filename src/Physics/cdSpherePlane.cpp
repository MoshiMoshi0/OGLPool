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

bool spherePlaneTest( Sphere* s, Plane* p, ContactInfo* info ){
	float dist = dot( p->normal, s->getPos() ) - dot( p->normal, p->pos );
	if( fabs( dist ) <= s->radius ){
		info->setContactTime( 0 );
		info->setContactPoint( s->getPos() - p->normal * s->radius );
		info->setDepth( s->radius - fabs( dist ) );
		info->setNormal( p->normal );
		info->setOverlapping( true );
		return true;
	}

	float denom = dot( p->normal, s->getVel() );
	if( denom * dist >= 0.0f ){
		return false;
	}

	float r = dist > 0.0f ? s->radius : -s->radius;
	float t = ( r - dist ) / denom;
	if( t > 0.15f || t < 0 ) return false;

	vec3 point = s->getPos() + t * s->getVel() - r * p->normal;

	info->setContactTime( t );
	info->setContactPoint( point );
	info->setNormal( p->normal );
	info->setColliding( true );
	return true;
}

} /* namespace Physics */
} /* namespace OGLPool */
