/*
 * crImpulse.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "ImpulseCollisionResponse.h"
#include <Physics/Constraint/ImpulseConstraint.h>
#include <vector>
#include <iostream>

using namespace std;

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

void addPenetrationConstraint( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3 n, float impDiag, ContactInfo* info, vector< ImpulseConstraint >* constraints ){
	ImpulseConstraint constraint;

	float deltaImpulse = impDiag;
	float deltaVel1Dotn	= dot( e0->getVel(),  info->normal ) + dot( e0->getAngVel(), cross( r0, info->normal ) );
	float deltaVel2Dotn	= dot( e1->getVel(), -info->normal ) + dot( e1->getAngVel(), cross( r1, info->normal ) );

	deltaImpulse -= deltaVel1Dotn*impDiag;
	deltaImpulse -= deltaVel2Dotn*impDiag;

	if( deltaImpulse < 0 ) deltaImpulse = 0.0f;
	//if( deltaImpulse > 1e10f ) deltaImpulse = 1e10f;

	constraint.impulse = -n * deltaImpulse;
	constraints->push_back( constraint );
}

void addContactConstraint( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3 n, float e, ContactInfo* info, vector< ImpulseConstraint >* constraints ){
	ImpulseConstraint constraint;

	vec3 a0 = cross( e0->angVel, r0 );
	vec3 a1 = cross( e1->angVel, r1 );
	vec3 v0 = e0->getVel() + a0;
	vec3 v1 = e1->getVel() + a1;
	vec3 dv = v1 - v0;

	float denom = e0->massInv + e1->massInv + dot( e0->getWorldInertiaInv() * cross(cross(r0,n),r0) + e1->getWorldInertiaInv() * cross(cross(r1,n),r1), n );

	float penetration = glm::max( info->depth - 0.01f, 0.f );
	float posError = penetration / info->deltaTime;

	float num = 0;
	if( penetration < 0.04 ){
		num = -(1+e) * dot(dv,n) - posError * 0.8f;
	}else{
		num = -(1+e) * dot(dv,n);

		addPenetrationConstraint( e0, e1, r0, r1, n, posError * 0.2f / denom, info, constraints );
	}

	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;
	//if( jr > 1e10f ) jr = 1e10f;

	constraint.impulse = -n * jr;
	constraints->push_back( constraint );
	//e0->applyImpulse( r0, -n * jr );
	//e1->applyImpulse( r1, n * jr );
}

void addFrictionConstraint( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3 n, float e, ContactInfo* info, vector< ImpulseConstraint >* constraints ){
	ImpulseConstraint constraint;

	vec3 a0 = cross( e0->angVel, r0 );
	vec3 a1 = cross( e1->angVel, r1 );
	vec3 v0 = e0->getVel() + a0;
	vec3 v1 = e1->getVel() + a1;
	vec3 dv = v1 - v0;

	float num = e * dot( dv, n );
	float denom = e0->massInv + e1->massInv + dot( e0->getWorldInertiaInv() * cross(cross(r0,n),r0) + e1->getWorldInertiaInv() * cross(cross(r1,n),r1), n );
	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;
	if( jr > 1e10f ) jr = 1e10f;

	constraint.impulse = n * jr;
	constraints->push_back( constraint );
	//e0->applyImpulse( r0, n * jr );
	//e1->applyImpulse( r1, -n * jr );
}

void addRollingFrictionConstraint( Entity* e0, Entity* e1, vec3 r0, vec3 r1, vec3 n, float e, ContactInfo* info, vector< ImpulseConstraint >* constraints ){
	ImpulseConstraint constraint;

	vec3 a0 = cross( e0->angVel, r0 );
	vec3 a1 = cross( e1->angVel, r1 );

	float num = e * (dot(n,a1) + dot(n,a0));
	float denom = dot( e0->getWorldInertiaInv() * n + e1->getWorldInertiaInv() * n, n );
	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;
	if( jr > 1e10f ) jr = 1e10f;

	constraint.impulse = n * jr;
	constraints->push_back( constraint );
	//e0->applyImpulse( r0, n * jr );
	//e1->applyImpulse( r1, -n * jr );
}

void response( ContactInfo* info ){
	if( !info->isValid() ) return;

	Entity* e0 = info->e0;
	Entity* e1 = info->e1;

	e0->pos += e0->getVel() * info->time * info->deltaTime;
	e1->pos += e1->getVel() * info->time * info->deltaTime;

	vec3 n = info->normal;
	vec3 r0 = info->point0 - e0->pos;
	vec3 r1 = info->point1 - e1->pos;

	vec3 da = e1->angVel - e0->angVel;
	vec3 dv = e1->vel - e0->vel;

	float e = 0.25;
	float df = 0.45;
	float sf = 0.45;
	float rf = 0.01;

	if( info->depth > 0.3 )
		cout << info->depth << endl;

	vector< ImpulseConstraint > constraints;
	addContactConstraint( e0, e1, r0, r1, n, e, info, &constraints );

	/*if( length( da ) > 1e30f ){
		da = normalize( da );
		if ( length(da) > 0.001f ) addRollingFrictionConstraint( e0, e1, r0, r1, da, rf, info, &constraints );
	}else{
		vec3 t0,t1;
		planeSpace( n, t0, t1 );

		addRollingFrictionConstraint( e0, e1, r0, r1, n, rf, info, &constraints );
		if ( length( t0 ) > 0.001f ) addRollingFrictionConstraint( e0, e1, r0, r1, t0, rf, info, &constraints );
		if ( length( t1 ) > 0.001f ) addRollingFrictionConstraint( e0, e1, r0, r1, t1, rf, info, &constraints );
	}*/

	/*vec3 t = dv - dot( dv, n ) * n;
	if( dot( t,t ) > epsilon<float>() ){
		vec3 t0 = normalize( t );
		//vec3 t1 = cross( n, t0 );
		addFrictionConstraint( e0, e1, r0, r1, t0, df, info, &constraints );
		//addFrictionConstraint( e0, e1, r0, r1, t1, df, info, &constraints );
	} else {
		vec3 t0,t1;
		planeSpace( n, t0, t1 );

		addFrictionConstraint( e0, e1, r0, r1, t0, sf, info, &constraints );
		addFrictionConstraint( e0, e1, r0, r1, t1, sf, info, &constraints );
	}*/

	for( int i = 0; i < constraints.size(); i++ ){
		e0->applyImpulse( r0, constraints[i].impulse );
		e1->applyImpulse( r1, -constraints[i].impulse );
	}
}

} /* namespace Physics */
} /* namespace OGLPool */
