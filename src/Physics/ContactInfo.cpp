/*
 * ContactInfo.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "ContactInfo.h"

namespace OGLPool {

ContactInfo::ContactInfo( float deltaTime ) : m_combinedFriction(0.f),m_combinedRollingFriction(0.f),m_combinedRestitution(0.f),
		m_lateralFrictionInitialized(false), m_appliedImpulse(0.f), m_appliedImpulseLateral1(0.f), m_appliedImpulseLateral2(0.f) {
	contactType = INVALID;

	e0 = e1 = 0;
	this->time = 0;
	this->deltaTime = deltaTime;
	this->depth = 0;
}

ContactInfo::~ContactInfo() {}
bool ContactInfo::isOverlapping(){ return contactType == OVERLAPPING; }
bool ContactInfo::isColliding(){ return contactType == COLLIDING; }
bool ContactInfo::isValid(){ return contactType != INVALID; }

void ContactInfo::setOverlapping( bool f ){ contactType = f ? OVERLAPPING : INVALID; }
void ContactInfo::setColliding( bool f ){ contactType = f ? COLLIDING : INVALID; }
void ContactInfo::setEntities( Entity* e0, Entity* e1 ){
	this->e0 = e0; this->e1 = e1;
	m_combinedRestitution = 0.5f;
	m_combinedRollingFriction = 0.2f;
	m_combinedFriction = 0.2f;
}

} /* namespace OGLPool */
