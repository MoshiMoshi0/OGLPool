/*
 * Camera.cpp
 *
 *  Created on: 16-03-2013
 *      Author: _CORE7
 */

#include "Camera.h"
#include <GL/glu.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace OGLPool {

Camera::Camera() {
	up = vec3(0, 1.0, 0);
	right = vec3( 1.0, 0, 0 );
	forward = vec3( 0, 0, -1.0 );
}

Camera::Camera( vec3 pos ) : Camera() {
	this->pos = pos;
}

Camera::~Camera() {}

void Camera::applyView(){
	gluLookAt( pos.x, pos.y, pos.z, pos.x + forward.x, pos.y + forward.y, pos.z + forward.z, up.x, up.y, up.z );
}

void Camera::setLookAt( vec3 lookAt ){
	vec3 newForward = normalize(lookAt - pos);
	vec3 axis = cross( forward, newForward );
	float angle = glm::angle( forward, newForward );

	rotate( axis, angle );
}

void Camera::move( vec3 dir, float speed ){
	pos += dir * speed;
}

void Camera::moveForward( float speed ){ move( forward, speed ); }
void Camera::moveUp( float speed ){ move( up, speed ); }
void Camera::moveRight( float speed ){ move( right, speed ); }

void Camera::rotate( vec3 axis, float angle ){
	quat rot = angleAxis( angle, axis );
	forward = normalize( rot * forward );
	right = normalize( rot * right );
	up = normalize( rot * up );
}

void Camera::rotateX( float angle ){ rotate( right, angle ); }
void Camera::rotateY( float angle ){ rotate( up, angle ); }
void Camera::rotateZ( float angle ){ rotate( forward, angle ); }

} /* namespace OGLPool */
