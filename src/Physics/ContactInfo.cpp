/*
 * ContactInfo.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "ContactInfo.h"

namespace OGLPool {

ContactInfo::ContactInfo() { contactType = INVALID; }

ContactInfo::~ContactInfo() {}

vec3 ContactInfo::getNormal(){ return normal; }
vec3 ContactInfo::getContactPoint(){ return point; }
float ContactInfo::getDepth(){ return depth; }
float ContactInfo::getContactTime(){ return time; }
bool ContactInfo::isOverlapping(){ return contactType == OVERLAPPING; }
bool ContactInfo::isColliding(){ return contactType == COLLIDING; }
bool ContactInfo::isValid(){ return contactType != INVALID; }

void ContactInfo::setNormal(vec3 normal){ this->normal = normal; }
void ContactInfo::setContactPoint(vec3 point){ this->point = point; }
void ContactInfo::setDepth(float depth){ this->depth = depth; }
void ContactInfo::setContactTime(float time){ this->time = time; }
void ContactInfo::setOverlapping( bool f ){ contactType = f ? OVERLAPPING : INVALID; }
void ContactInfo::setColliding( bool f ){ contactType = f ? COLLIDING : INVALID; }

} /* namespace OGLPool */
