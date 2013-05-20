/*
 * cdSpherePlane.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "cdSpherePlane.h"
#include <iostream>
using namespace std;

namespace OGLPool {
namespace Physics {

bool spherePlaneIntersection(){
	return false;
}

bool spherePlaneSwept( Sphere* s, Plane* p, ManifoldPoint* info ){
	vec3 vel = s->linVel * info->deltaTime;
	float dist = dot( p->normal, s->pos ) - dot( p->normal, p->pos );
	float denom = dot( p->normal, vel );
	if( denom * dist >= 0.0f ){
		return false;
	}

	float r = dist > 0.0f ? s->radius : -s->radius;
	float t = ( r - dist ) / denom;
	if( t < 0 || t > 1 ) return false;

	vec3 point = s->pos + t * vel - r * p->normal;

	info->time = t;
	info->point0 = point;
	info->point1 = point;
	info->normal = p->normal;
	return true;
}

bool spherePlaneOverlap( Sphere* s, Plane* p, ManifoldPoint* info ){
	float dist = dot( p->normal, s->pos ) - dot( p->normal, p->pos );
	if( dist > s->radius ) return false;

	float penetration = s->radius - fabs( dist );
	info->time = 0;
	info->point0 = s->pos - p->normal * s->radius;
	info->point1 = s->pos - p->normal * (s->radius - penetration);
	info->depth = -penetration;
	info->normal = p->normal;
	return true;
}

bool spherePlaneTest( Sphere* s, Plane* p, ManifoldPoint* info ){
	info->setBodies( s, p );
	if( spherePlaneOverlap( s, p, info ) ) return true;
	return spherePlaneSwept( s, p, info );
}

} /* namespace Physics */
} /* namespace OGLPool */
