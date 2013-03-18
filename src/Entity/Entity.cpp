/*
 * Entity.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "Entity.h"
#include <glm/gtx/quaternion.hpp>

namespace OGLPool {
int Entity::s_id = 0;

Entity::Entity() {
	id = Entity::s_id++;
	setMass( 1 );
}

Entity::~Entity() {
}

Entity::Entity( vec3 pos ) : Entity() {
	this->pos = pos;
}

vec3 Entity::toWorldAxis( const vec3& v ) const { return rot * v;}
vec3 Entity::toWorld( const vec3& v ) const { return rot * v + pos;}
mat3 Entity::toWorld( const mat3& mat ) const {
	mat3 mRot = toMat3( rot );
	return mRot * mat * inverse( mat );
}

void Entity::applyForce( const vec3& f ){ force += f;}
void Entity::applyOffsetForce( const vec3& f, const vec3& p ){ applyForce( f ); applyTorque( f, p );}
void Entity::applyTorque( const vec3& f, const vec3& p ){ torque += cross( f, p - pos ); }

vec3 Entity::getAngVel() const { return angVel; }
void Entity::setAngVel(vec3 angVel) {this->angVel = angVel;}

vec3 Entity::getForce() const {return force;}
void Entity::setForce(vec3 force) {	this->force = force;}

mat3 Entity::getInertia() const {	return inertia;}
mat3 Entity::getWorldInertia() const { return toWorld( inertia ); }
void Entity::setInertia(mat3 inertia) {this->inertia = inertia; this->inertiaInv = inverse( inertia ); }
mat3 Entity::getInertiaInv() const {	return inertiaInv;}
mat3 Entity::getWorldInertiaInv() const { return toWorld( inertiaInv ); }

float Entity::getMass() const {	return mass;}
void Entity::setMass(float mass) {this->mass = mass; this->massInv = ( mass > 0 ? 1.f/mass : 0.0f ); }
float Entity::getMassInv() const {	return massInv;}

vec3 Entity::getPos() const { return pos;}
void Entity::setPos(vec3 pos) {	this->pos = pos;}

quat Entity::getRot() const { return rot;}
void Entity::setRot(quat rot) { this->rot = rot; }

vec3 Entity::getTorque() const { return torque;}
void Entity::setTorque(vec3 torque) { this->torque = torque;}

vec3 Entity::getVel() const { return vel;}
void Entity::setVel(vec3 vel) {	this->vel = vel;}

} /* namespace OGLPool */
