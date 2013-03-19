/*
 * ContactInfo.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "ContactInfo.h"

namespace OGLPool {

ContactInfo::ContactInfo() {}

ContactInfo::~ContactInfo() {}

vec3 ContactInfo::getNormal(){ return normal; }
vec3 ContactInfo::getContactPoint(){ return point; }
float ContactInfo::getDepth(){ return depth; }
float ContactInfo::getContactTime(){ return time; }

void ContactInfo::setNormal(vec3 normal){ this->normal = normal; }
void ContactInfo::setContactPoint(vec3 point){ this->point = point; }
void ContactInfo::setDepth(float depth){ this->depth = depth; }
void ContactInfo::setContactTime(float time){ this->time = time; }

} /* namespace OGLPool */
