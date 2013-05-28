/*
 * ContactManifold.h
 *
 *  Created on: 25-05-2013
 *      Author: _CORE7
 */

#ifndef CONTACTMANIFOLD_H_
#define CONTACTMANIFOLD_H_

#include "ManifoldPoint.h"
#include <array>
using namespace std;

namespace OGLPool {

constexpr static uint MAX_NUM_POINTS = 4;
class ContactManifold {
public:

	ContactManifold();
	virtual ~ContactManifold();

	int areaSort( ManifoldPoint* pt );
	int addManifoldPoint( ManifoldPoint* pt );
	void reset();

	void addContact( const vec3& point, const vec3& normal, float depth );
	void addContact( const vec3& point0, const vec3& point1, const vec3& normal, float depth );
	void addContact( const vec3& point, const vec3& normal, float depth, float time );
	void addContact( const vec3& point0, const vec3& point1, const vec3& normal, float depth, float time );

	void setBodies( RigidBody* e0, RigidBody* e1, bool lock = true );

	void replaceManifoldPoint( ManifoldPoint* pt, int index );
	int getCacheEntry( ManifoldPoint* pt );

	RigidBody* e0;
	RigidBody* e1;

	int pointsAdded;
	array< ManifoldPoint*, MAX_NUM_POINTS> points;

	float combinedRestitution;
	float combinedRollingFriction;
	float combinedFriction;
	float contactBreakingTreshold;
	bool bodiesLocked;
};

} /* namespace OGLPool */
#endif /* CONTACTMANIFOLD_H_ */
