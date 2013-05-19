/*
 * ContactInfo.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "ManifoldPoint.h"

namespace OGLPool {

ManifoldPoint::ManifoldPoint( float deltaTime ) : deltaTime( deltaTime ) {
	reset();

	m_lateralFrictionDir1 = vec3();
	m_lateralFrictionDir2 = vec3();

	m_combinedFriction = 0.0f;
	m_combinedRollingFriction = 0.0f;
	m_combinedRestitution = 0.0f;

	m_lateralFrictionInitialized = false;
	m_appliedImpulse = 0.0f;
	m_appliedImpulseLateral1 = 0.0f;
	m_appliedImpulseLateral2 = 0.0f;
}

ManifoldPoint::~ManifoldPoint() {}

void ManifoldPoint::setEntities( RigidBody* e0, RigidBody* e1 ){
	this->e0 = e0; this->e1 = e1;
	m_combinedRestitution = e0->restitutionCoef * e1->restitutionCoef;
	m_combinedRollingFriction = e0->frictionCoef * e1->frictionCoef;
	m_combinedFriction = e0->rollingFrictionCoef * e1->rollingFrictionCoef;
}

void ManifoldPoint::reset(){
	point0 = vec3();
	point1 = vec3();
	normal = vec3();

	e0 = e1 = 0;
	time = 0;
	depth = 0;
}

} /* namespace OGLPool */
