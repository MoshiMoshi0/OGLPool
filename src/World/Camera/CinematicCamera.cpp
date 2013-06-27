/*
 * CinematicCamera.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "CinematicCamera.h"

namespace OGLPool {

CinematicCamera::CinematicCamera() : Camera() {}
CinematicCamera::CinematicCamera( vec3 pos ) : Camera( pos ) {}

CinematicCamera::~CinematicCamera(){}

void CinematicCamera::setLookAt( vec3 lookAt ){
	forward = normalize(lookAt - pos);
	right = cross( forward, vec3(0,1,0) );
}

void CinematicCamera::rotateY( float angle ){
	rotate( vec3(0,1,0), angle );
}

void CinematicCamera::moveUp( float speed ){
	move( vec3(0,1,0), speed );
}

} /* namespace OGLPool */
