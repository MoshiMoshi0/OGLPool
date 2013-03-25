/*
 * crImpulse.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "ImpulseCollisionResponse.h"

namespace OGLPool {
namespace Physics {

void planeSpace( const vec3& n, vec3& t0, vec3& t1 ){
	if( fabs( n.z ) > sqrtf( 1/2 ) ){
		float a = n.y * n.y + n.z * n.z;
		float k = 1.0f/sqrt( a );

		t0 = vec3(0, -n.z * k, n.y * k);
		t1 = vec3(a*k, -n.x * t0.z, n.x * t0.y);
	}else{
		float a = n.x * n.x + n.y * n.y;
		float k = 1.0f/sqrt( a );

		t0 = vec3(-n.y*k, n.x * k, 0);
		t1 = vec3(-n.z * t0.y, n.z * t0.x, a*k);
	}
}

void applyContactImpulse( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3& dv, vec3& da, vec3 n, float e ){
	float num = (1+e) * dot(dv,n);
	float denom = e0->massInv + e1->massInv + dot( cross(r0,n), e0->getWorldInertiaInv() * cross(r0,n) ) + dot( cross(r1,n), e1->getWorldInertiaInv() * cross(r1,n) );
	float jr = num / denom;

	if( jr > 0 ) jr = 0.0f;

	e0->applyImpulse( r0, n * jr );
	e1->applyImpulse( r1, -n * jr );

	vec3 a0 = cross( e0->angVel, r0 );
	vec3 a1 = cross( e1->angVel, r1 );
	vec3 v0 = e0->getVel() + a0;
	vec3 v1 = e1->getVel() + a1;

	da = a1 - a0;
	dv = v1 - v0;
}

void applyFrictionImpulse( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3& dv, vec3& da, vec3 n, float e ){
	float num = e * dot(dv,n);
	float denom = e0->massInv + e1->massInv + dot( e0->getWorldInertiaInv() * cross(cross(r0,n),r0) + e1->getWorldInertiaInv() * cross(cross(r1,n),r1), n );//dot( cross(r0,n), e0->getWorldInertiaInv() * cross(r0,n) ) + dot( cross(r1,n), e1->getWorldInertiaInv() * cross(r1,n) );
	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;

	e0->applyImpulse( r0, n * jr );
	e1->applyImpulse( r1, -n * jr );

	vec3 a0 = cross( e0->angVel, r0 );
	vec3 a1 = cross( e1->angVel, r1 );
	vec3 v0 = e0->getVel() + a0;
	vec3 v1 = e1->getVel() + a1;

	da = a1 - a0;
	dv = v1 - v0;
}

void applyRollingFrictionImpulse( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3& dv, vec3& da, vec3 n, float e ){
	float num = e * dot(da,n);
	float denom = e0->massInv + e1->massInv + dot( e0->getWorldInertiaInv() * n + e1->getWorldInertiaInv() * n, n );
	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;

	e0->angVel += e0->getWorldInertiaInv() * cross(r0, n * jr );
	e1->angVel += e1->getWorldInertiaInv() * cross(r1, -n * jr);

	vec3 a0 = cross( e0->angVel, r0 );
	vec3 a1 = cross( e1->angVel, r1 );
	vec3 v0 = e0->getVel() + a0;
	vec3 v1 = e1->getVel() + a1;

	da = a1 - a0;
	dv = v1 - v0;
}

void response( Entity* e0, Entity* e1, ContactInfo* info ){
	if( !info->isValid() ) return;
	if( info->isOverlapping() ){
		float m0 = e0->massInv / (e0->massInv + e1->massInv);
		float m1 = e1->massInv / (e0->massInv + e1->massInv);
		float d0 = m0 * info->getDepth();
		float d1 = m1 * info->getDepth();
		e0->pos -= d0 * info->getNormal();
		e1->pos += d1 * info->getNormal();
	}

	e0->pos += e0->getVel() * info->getContactTime();
	e1->pos += e1->getVel() * info->getContactTime();

	vec3 n = info->getNormal();
	vec3 r0 = info->getContactPoint() - e0->pos;
	vec3 r1 = info->getContactPoint() - e1->pos;

	vec3 a0 = cross( e0->angVel, r0 );
	vec3 a1 = cross( e1->angVel, r1 );
	vec3 v0 = e0->getVel() + a0;
	vec3 v1 = e1->getVel() + a1;

	vec3 da = a1 - a0;
	vec3 dv = v1 - v0;

	float e = 0.5;
	float df = 0.5;
	float sf = 0.5;
	float rf = 0.01;

	applyContactImpulse( e0, e1, r0, r1, dv, da, n, e );

	if( length( da ) > 1e30f ){
		da = normalize( da );
		if ( length(da) > 0.001f ) applyRollingFrictionImpulse( e0, e1, r0, r1, dv, da, da, rf );
	}else{
		vec3 t0,t1;
		planeSpace( n, t0, t1 );

		applyRollingFrictionImpulse( e0, e1, r0, r1, dv, da, n, rf );
		if ( length( t0 ) > 0.001f ) applyRollingFrictionImpulse( e0, e1, r0, r1, dv, da, t0, rf );
		if ( length( t1 ) > 0.001f ) applyRollingFrictionImpulse( e0, e1, r0, r1, dv, da, t1, rf );
	}

	vec3 t = dv - dot( dv, n ) * n;
	if( dot( t,t ) > epsilon<float>() ){
		vec3 t0 = normalize( t );
		vec3 t1 = cross( n, t0 );
		applyFrictionImpulse( e0, e1, r0, r1, dv, da, t0, df );
		applyFrictionImpulse( e0, e1, r0, r1, dv, da, t1, df );
	} else {
		vec3 t0,t1;
		planeSpace( n, t0, t1 );

		applyFrictionImpulse( e0, e1, r0, r1, dv, da, t0, sf );
		applyFrictionImpulse( e0, e1, r0, r1, dv, da, t1, sf );
	}
}

} /* namespace Physics */
} /* namespace OGLPool */
