/*
 * SpherePlaneTester.cpp
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#include "SpherePlaneTester.h"

namespace OGLPool {

SpherePlaneTester::SpherePlaneTester(){}
SpherePlaneTester::SpherePlaneTester( Sphere* s, Plane* p ){ setBodies( s, p ); }

SpherePlaneTester::~SpherePlaneTester(){}

bool SpherePlaneTester::overlapTest( ManifoldPoint* info ){
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

bool SpherePlaneTester::sweepTest( ManifoldPoint* info ){
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

void SpherePlaneTester::setBodies( Sphere* s, Plane* p ){ this->s = s; this->p = p; CollisionTester::setBodies( s, p ); }

} /* namespace OGLPool */
