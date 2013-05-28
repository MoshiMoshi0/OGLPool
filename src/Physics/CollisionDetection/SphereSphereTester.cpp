/*
 * SphereSphereTester.cpp
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#include "SphereSphereTester.h"
#include <App/Time.h>

namespace OGLPool {

SphereSphereTester::SphereSphereTester(){}
SphereSphereTester::SphereSphereTester( Sphere* s0, Sphere* s1 ){ setBodies( s0, s1 ); }

SphereSphereTester::~SphereSphereTester(){}

bool SphereSphereTester::overlapTest( ContactManifold* info ){
	vec3 dp = s0->pos - s1->pos;
	float r = s0->radius + s1->radius;
	if( dot( dp, dp ) <= r*r ){
		vec3 n = normalize( dp );
		info->addContact( 0.5f * ( s0->pos + s1->pos ), n, -(r - length( dp )) );
		return true;
	}
	return false;
}

bool SphereSphereTester::sweptTest( ContactManifold* info ){
	vec3 dv = (s1->linVel - s0->linVel) * Time::DELTA_TIME;
	vec3 dp = s1->pos - s0->pos;

	float a = dot( dv, dv );
	float c = dot( dp, dp );

	float rSum = s0->radius + s1->radius;
	float rSumSqr = rSum*rSum;

	if (a <= 0) return false;

	float b = dot(dp, dv);
	if (b > 0) return false;

	if (-a <= b || (a + 2.0f*b) + c <= rSumSqr){
		float cdiff = c - rSumSqr;
		float discr = b*b - a*cdiff;
		if (discr < 0) return false;
		if (cdiff <= 0) return false;

		float t = -(b + sqrt(discr))/a;
		if( t < 0 || t > 1 ) return false;

		vec3 vcd = dp + t*dv;
		vec3 point = s0->pos + t * s0->linVel + (s0->radius / rSum) * vcd;
		info->addContact( point, normalize( dp ), 0, t );
		return true;
	}

	return false;
}

void SphereSphereTester::setBodies( Sphere* s0, Sphere* s1 ){ this->s0 = s0; this->s1 = s1; CollisionTester::setBodies( s0, s1 ); }

} /* namespace OGLPool */
