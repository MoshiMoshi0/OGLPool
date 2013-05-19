/*
 * cdSphereSphere.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "cdSphereSphere.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>
using namespace std;
using namespace glm;

namespace OGLPool {
namespace Physics {

bool sphereSphereIntersection( Sphere* s0, Sphere* s1 ){
	return length(s0->pos + s1->pos) <= s0->radius + s1->radius;
}

bool sphereSphereOverlap( Sphere* s0, Sphere* s1, ManifoldPoint* info ){
	vec3 dp = s0->pos - s1->pos;
	float r = s0->radius + s1->radius;
	if( dot( dp, dp ) <= r*r ){
		vec3 n = normalize( dp );
		//info->point0 = s0->pos + n * s0->radius;
		///info->point1 = s1->pos - n * s1->radius;
		info->point0 = 0.5f * ( s0->pos + s1->pos );
		info->point1 = 0.5f * ( s0->pos + s1->pos );
		info->depth = -(r - length( dp ));
		info->normal = n;
		return true;
	}
	return false;
}

bool sphereSphereSwept( Sphere* s0, Sphere* s1, ManifoldPoint* info ){
	vec3 relVelocity = (s1->linVel - s0->linVel) * info->deltaTime;
	float a = dot( relVelocity, relVelocity );
	vec3 CDiff = s1->pos - s0->pos;
	float c = dot( CDiff, CDiff );
	float rSum = s0->radius + s1->radius;
	float rSumSqr = rSum*rSum;

	if (a > 0){
		float b = dot(CDiff, relVelocity);
		if (b <= 0){
			if (-a <= b || (a + 2.0f*b) + c <= rSumSqr){
				float cdiff = c - rSumSqr;
				float discr = b*b - a*cdiff;
				if (discr >= 0){
					if (cdiff <= 0) return false;

					float mContactTime = -(b + sqrt(discr))/a;
					if( mContactTime < 0 || mContactTime > 1 ) return false;

					vec3 newCDiff = CDiff + mContactTime*relVelocity;

					info->time = mContactTime;
					info->normal = normalize( CDiff );
					info->point0 = s0->pos + mContactTime*s0->linVel + (s0->radius/rSum)*newCDiff;
					info->point1 = s0->pos + mContactTime*s0->linVel + (s0->radius/rSum)*newCDiff;
					return true;
				}
			}
			return false;
		}
	}

	return false;
}

bool sphereSphereTest( Sphere* s0, Sphere* s1, ManifoldPoint* info ){
	info->setEntities( s0, s1 );
	if( sphereSphereOverlap( s0, s1, info ) ) return true;
	return sphereSphereSwept( s0, s1, info );
}

} /* namespace Physics */
} /* namespace OGLPool */
