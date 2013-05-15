/*
 * ContactInfo.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "ContactInfo.h"

namespace OGLPool {

ContactInfo::ContactInfo( float deltaTime ) {
	contactType = INVALID;

	this->deltaTime = deltaTime;
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
	m_combinedRollingFriction = 0.5f;
	m_combinedFriction = 0.5f;//e0->friction * e1->friction
}

} /* namespace OGLPool */
