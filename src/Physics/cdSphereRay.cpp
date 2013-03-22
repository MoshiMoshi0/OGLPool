/*
 * cdSphereRay.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "cdSphereRay.h"

namespace OGLPool {
namespace Physics {

bool sphereRayTest( vec3 rayOrigin, vec3 rayDir, Sphere* s, ContactInfo* info ){
	return sphereRayTest( rayOrigin, rayDir, s->pos, s->radius, info );
}

bool sphereRayIntersection( vec3 rayOrigin, vec3 rayDir, vec3 pos, float radius ){
	float a = dot(rayDir, rayDir);
	float b = 2 * dot(rayDir, rayOrigin);
	float c = dot(rayOrigin, rayOrigin) - radius * radius;

	float disc = b * b - 4 * a * c;

	if (disc < 0)
		return false;

	float distSqrt = sqrtf(disc);
	float q;
	if (b < 0) q = (-b - distSqrt)/2.0;
	else q = (-b + distSqrt)/2.0;

	float t0 = q / a;
	float t1 = c / q;

	if (t0 > t1) {
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	if ( t1 < 0 )
		return false;

	return true;
}

bool sphereRayIntersection( vec3 rayOrigin, vec3 rayDir, Sphere* s ){
	return sphereRayIntersection( rayOrigin, rayDir, s->pos, s->radius );
}

bool sphereRayTest( vec3 rayOrigin, vec3 rayDir, vec3 pos, float radius, ContactInfo* info ){
	if( info == 0 )
		return false;

	vec3 d = rayOrigin - pos;
	float a = dot(rayDir, rayDir);
	float b = 2 * dot(rayDir, d);
	float c = dot(d, d) - radius * radius;

	float disc = b * b - 4 * a * c;

	if (disc < 0)
		return false;

	float distSqrt = sqrtf(disc);
	float q;
	if (b < 0) q = (-b - distSqrt)/2.0;
	else q = (-b + distSqrt)/2.0;

	float t0 = q / a;
	float t1 = c / q;

	if (t0 > t1) {
		float temp = t0;
		t0 = t1;
		t1 = temp;
	}

	if ( t1 < 0 )
		return false;

	float t = t0 < 0 ? t1 : t0;
	if( t > 0.15f || t < 0 ) return false;

	vec3 point = rayOrigin + rayDir * t;
	info->setContactTime( t );
	info->setContactPoint( point );
	info->setNormal( normalize( point - pos ) );
	info->setColliding( true );
	return true;
}

} /* namespace Physics */
} /* namespace OGLPool */
