/*
 * Plane.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "Plane.h"
#include <SFML/OpenGL.hpp>
#include <cfloat>
#include <Util/UDebug/Debug.h>
#include <Util/MathUtil.h>

namespace OGLPool {

Plane::Plane( vec3 normal, vec3 point ) : RigidBody( point ) {
	this->normal = normalize( normal );
	setInertia( mat3(0.0f) );

	restitutionCoef = 0.6f;
	frictionCoef = 0.5f;
	rollingFrictionCoef = 0.5f;

	float skin = 0.1f;
	boundingBox.skin = skin;

	vec3 t0,t1;
	planeSpace( normal, t0, t1 );
	boundingBox.min = -abs(t0 + t1) * 50.f - normal * skin; //std::numeric_limits<float>::max()
	boundingBox.max =  abs(t0 + t1) * 50.f + normal * skin;
}

Plane::~Plane() {}

void Plane::render(){
	auto debugDraw = Debug::getDebugDraw();
	debugDraw->setColor(1,1,1)->drawPlane( pos, normal, 50 );
	debugDraw->setColor(0,0,1)->drawVector( pos, normal * 10.0f );
}

} /* namespace OGLPool */
