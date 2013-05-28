/*
 * ContactManifold.cpp
 *
 *  Created on: 25-05-2013
 *      Author: _CORE7
 */

#include "ContactManifold.h"
#include <glm/glm.hpp>
#include <limits>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
using namespace std;

namespace OGLPool {

ContactManifold::ContactManifold(){
	pointsAdded = 0;
	for( uint i = 0; i < MAX_NUM_POINTS; i++ ){
		points[i] = 0;
	}

	bodiesLocked = false;
}

ContactManifold::~ContactManifold(){
	for( uint i = 0; i < MAX_NUM_POINTS; i++ ){
		if( points[i] ) delete points[i];
	}
}

int ContactManifold::areaSort( ManifoldPoint* pt ){
	int maxPenetrationIndex = -1;

	float maxPenetration = pt->depth;
	assert( pointsAdded >= 4 );
	for (int i = 0; i < 4; i++) {
		if (points[i]->depth < maxPenetration) {
			maxPenetrationIndex = i;
			maxPenetration = points[i]->depth;
		}
	}

	float res0,res1,res2,res3;
	if (maxPenetrationIndex != 0) {
		vec3 a0 = pt->local0 - points[1]->local0;
		vec3 b0 = points[3]->local0 - points[2]->local0;
		vec3 cr = cross(a0, b0);
		res0 = dot( cr, cr );
	}

	if (maxPenetrationIndex != 1) {
		vec3 a1 = pt->local0 - points[0]->local0;
		vec3 b1 = points[3]->local0 - points[2]->local0;
		vec3 cr = cross(a1, b1);
		res1 = dot( cr, cr );
	}

	if (maxPenetrationIndex != 2) {
		vec3 a2 = pt->local0 - points[0]->local0;
		vec3 b2 = points[3]->local0 - points[1]->local0;
		vec3 cr = cross(a2, b2);
		res2 = dot( cr, cr );
	}

	if (maxPenetrationIndex != 3) {
		vec3 a3 = pt->local0 - points[0]->local0;
		vec3 b3 = points[2]->local0 - points[1]->local0;
		vec3 cr = cross(a3, b3);
		res3 = dot( cr, cr );
	}

	float max = res0;
	uint maxIndex = 0;

	if( res1 > max ){
		max = res1;
		maxIndex = 1;
	}
	if( res2 > max ){
		max = res2;
		maxIndex = 2;
	}
	if( res3 > max ){
		max = res3;
		maxIndex = 3;
	}

	return maxIndex;
}

void ContactManifold::replaceManifoldPoint( ManifoldPoint* pt, int index ){
	assert( index < pointsAdded );
	ManifoldPoint* rp = points[ index ];

	float appliedImpulse = rp->appliedImpulse;
	float appliedImpulseLateral1 = rp->appliedImpulseLateral1;
	float appliedImpulseLateral2 = rp->appliedImpulseLateral2;

	delete rp;
	points[ index ] = pt;
	pt->appliedImpulse = appliedImpulse;
	pt->appliedImpulseLateral1 = appliedImpulseLateral1;
	pt->appliedImpulseLateral2 = appliedImpulseLateral2;
}

int ContactManifold::getCacheEntry( ManifoldPoint* pt ){
	float shortestDist = 0.02f * contactBreakingTreshold * contactBreakingTreshold;// getContactBreakingThreshold() * getContactBreakingThreshold();

	int nearestPoint = -1;
	for( int i = 0; i < pointsAdded; i++ ) {
		ManifoldPoint* mp = points[i];

		vec3 diffA = mp->local0 - pt->local0;
		const float distToManiPoint = dot( diffA, diffA );
		if( distToManiPoint < shortestDist ) {
			shortestDist = distToManiPoint;
			nearestPoint = i;
		}
	}

	return nearestPoint;
}

int ContactManifold::addManifoldPoint( ManifoldPoint* pt ){
	int index = pointsAdded;
	if( index == MAX_NUM_POINTS ){
		index = areaSort( pt );
	}else{
		pointsAdded++;
	}

	if( points[ index ] ){
		delete points[ index ];
	}
	points[ index ] = pt;
	return index;
}

void ContactManifold::reset(){
	bodiesLocked = false;
}

void ContactManifold::addContact( const vec3& point, const vec3& normal, float depth ){ addContact( point, point, normal, depth, 0 ); }
void ContactManifold::addContact( const vec3& point0, const vec3& point1, const vec3& normal, float depth ){ addContact( point0, point1, normal, depth, 0 ); }
void ContactManifold::addContact( const vec3& point, const vec3& normal, float depth, float time ){ addContact( point, point, normal, depth,time ); }
void ContactManifold::addContact( const vec3& point0, const vec3& point1, const vec3& normal, float depth, float time ){
	ManifoldPoint* pt = new ManifoldPoint();
	pt->point0 = point0;
	pt->point1 = point1;
	pt->normal = normal;
	pt->depth = depth;
	pt->time = time;
	pt->combinedFriction = combinedFriction;
	pt->combinedRestitution = combinedRestitution;
	pt->combinedRollingFriction = combinedRollingFriction;
	pt->local0 = e0 ? e0->toLocal( point0 ) : vec3();
	pt->local1 = e1 ? e1->toLocal( point1 ) : vec3();

	int index = getCacheEntry( pt );
	if( index >= 0 ){
		replaceManifoldPoint( pt, index );
	}else{
		index = addManifoldPoint( pt );
	}
}

void ContactManifold::setBodies( RigidBody* e0, RigidBody* e1, bool lock ){
	if( bodiesLocked ) return;
	bodiesLocked = lock;
	this->e0 = e0; this->e1 = e1;

	float restitution0 = e0 ? e0->restitutionCoef : 1;
	float friction0 = e0 ? e0->frictionCoef : 1;
	float rolling0 = e0 ? e0->rollingFrictionCoef : 1;
	float radius0 = e0 ? e0->getBoundingSphereRadius() : numeric_limits<float>::max();
	float restitution1 = e1 ? e1->restitutionCoef : 1;
	float friction1 = e1 ? e1->frictionCoef : 1;
	float rolling1 = e1 ? e1->rollingFrictionCoef : 1;
	float radius1 = e1 ? e1->getBoundingSphereRadius() : numeric_limits<float>::max();

	combinedRestitution = restitution0 * restitution1;
	combinedFriction = friction0 * friction1;
	combinedRollingFriction = rolling0 * rolling1;
	contactBreakingTreshold = 0.1f * glm::min( radius0, radius1 );
}

} /* namespace OGLPool */
