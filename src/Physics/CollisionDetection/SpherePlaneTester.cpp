/*
 * SpherePlaneTester.cpp
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#include "SpherePlaneTester.h"
#include <App/Time.h>

namespace OGLPool {

SpherePlaneTester::SpherePlaneTester(){}
SpherePlaneTester::SpherePlaneTester( Sphere* s, Plane* p ){ setBodies( s, p ); }

SpherePlaneTester::~SpherePlaneTester(){}

bool SpherePlaneTester::overlapTest( ContactManifold* info ){
	float dist = dot( p->normal, s->pos ) - dot( p->normal, p->pos );
	if( dist > s->radius ) return false;

	float penetration = s->radius - fabs( dist );
	vec3 p0 = s->pos - p->normal * s->radius;
	vec3 p1 = s->pos - p->normal * (s->radius - penetration);
	info->addContact( p0, p1, p->normal, -penetration );
	return true;
}

bool SpherePlaneTester::sweptTest( ContactManifold* info ){
	assert(0);
	return false;
	/*vec3 vel = s->linVel * Time::DELTA_TIME;
	float dist = dot( p->normal, s->pos ) - dot( p->normal, p->pos );
	float denom = dot( p->normal, vel );
	if( denom * dist >= 0.0f ){
		return false;
	}

	float r = dist > 0.0f ? s->radius : -s->radius;
	float t = ( r - dist ) / denom;
	if( t < 0 || t > 1 ) return false;

	vec3 point = s->pos + t * vel - r * p->normal;
	info->addContact( point, p->normal, 0, t );
	return true;*/
}

void SpherePlaneTester::setBodies( Sphere* s, Plane* p ){ this->s = s; this->p = p; CollisionTester::setBodies( s, p ); }

} /* namespace OGLPool */
