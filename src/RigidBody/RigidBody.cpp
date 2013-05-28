/*
 * Entity.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "RigidBody.h"
#include <SFML/OpenGL.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

namespace OGLPool {
int RigidBody::s_id = 0;

RigidBody::RigidBody() {
	id = RigidBody::s_id++;
	linearDamping = 0.05f;
	angularDamping = 0.1f;
	setMass( 0 );
}

RigidBody::~RigidBody() {
}

RigidBody::RigidBody( vec3 pos ) : RigidBody() {
	this->pos = pos;
}

RigidBody::RigidBody( RigidBody* e ) : RigidBody() {
	mass = e->mass;
	massInv = e->massInv;
	pos = e->pos;
	linVel = e->linVel;
	force = e->force;

	rot = e->rot;
	angVel = e->angVel;
	torque = e->torque;

	inertia = e->inertia;
	inertiaInv = e->inertiaInv;
}

vec3 RigidBody::toWorldAxis( const vec3& v ) const { return rot * v; }
vec3 RigidBody::toWorld( const vec3& v ) const { return rot * v + pos; }
mat3 RigidBody::toWorld( const mat3& mat ) const {
	mat3 mRot = toMat3( rot );
	return mRot * mat * inverse( mRot );
}

vec3 RigidBody::toLocal( const vec3& v ) const { return toLocalAxis( v - pos ); }
vec3 RigidBody::toLocalAxis( const vec3& v ) const { return transpose( toMat3( rot ) ) * v; }

void RigidBody::applyTransform(){
	glMultMatrixf( value_ptr( translate( mat4(1.0f), pos) * toMat4( rot ) ) );
}
void RigidBody::applyForce( const vec3& f ){ force += f; }
void RigidBody::applyOffsetForce( const vec3& f, const vec3& p ){ applyForce( f ); applyTorque( f, p ); }
void RigidBody::applyTorque( const vec3& f, const vec3& p ){ torque += cross( f, p ); }
void RigidBody::applyImpulse( const vec3& r, const vec3& i ){
	linVel += massInv * i;
	angVel += getWorldInertiaInv() * cross(r, i);
}
void RigidBody::applyDamping( float dt ){
	linVel *= powf( 1.0f - linearDamping, dt);
	angVel *= powf( 1.0f - angularDamping, dt);
}

vec3 RigidBody::getAngVel() const { return angVel; }
void RigidBody::setAngVel( vec3 angVel ) { this->angVel = angVel; }

vec3 RigidBody::getForce() const { return force; }
void RigidBody::setForce( vec3 force ) { this->force = force; }

mat3 RigidBody::getInertia() const { return inertia; }
mat3 RigidBody::getWorldInertia() const { return toWorld( inertia ); }
void RigidBody::setInertia( mat3 inertia ) {
	this->inertia = inertia;
	if(determinant( inertia ) != 0){
		this->inertiaInv = inverse( inertia );
	}else{
		this->inertiaInv = mat3(0.0f);
	}
}
mat3 RigidBody::getInertiaInv() const { return inertiaInv; }
mat3 RigidBody::getWorldInertiaInv() const { return toWorld( inertiaInv ); }

float RigidBody::getMass() const { return mass; }
void RigidBody::setMass( float mass ) {this->mass = mass; this->massInv = ( mass > 0 ? 1.f/mass : 0.0f ); }
float RigidBody::getMassInv() const { return massInv; }

vec3 RigidBody::getPos() const { return pos; }
void RigidBody::setPos( vec3 pos ) { this->pos = pos; }

quat RigidBody::getRot() const { return rot; }
void RigidBody::setRot( quat rot ) { this->rot = rot; }

vec3 RigidBody::getTorque() const { return torque; }
void RigidBody::setTorque( vec3 torque ) { this->torque = torque; }

vec3 RigidBody::getLinVel() const { return linVel; }
void RigidBody::setLinVel( vec3 vel ) { this->linVel = vel; }

const BoundingBox& RigidBody::getBoundingBox() {
	boundingBox.pos = pos;
	boundingBox.minDyn = -abs(linVel) / 60.f;
	boundingBox.maxDyn = abs(linVel) / 60.f;
	return boundingBox;
}

} /* namespace OGLPool */
