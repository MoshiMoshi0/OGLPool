/*
 * FpsCamera.cpp
 *
 *  Created on: 16-03-2013
 *      Author: _CORE7
 */

#include "FpsCamera.h"
#include <iostream>
#include <App/Input.h>

using namespace OGLPool::IO;
namespace OGLPool {

FpsCamera::FpsCamera() : Camera() {}
FpsCamera::FpsCamera( vec3 pos ) : Camera( pos ) {}
FpsCamera::~FpsCamera() {}

void FpsCamera::update( float dt ){
	rotateX( -IO::Input::getMouseDY() );
	rotateY( -IO::Input::getMouseDX() );

	int fd = Input::isKeyPressed( IO::Input::W ) - Input::isKeyPressed( IO::Input::S );
	int sd = Input::isKeyPressed( IO::Input::D ) - Input::isKeyPressed( IO::Input::A );
	int ud = Input::isKeyPressed( IO::Input::Space ) - Input::isKeyPressed( IO::Input::LControl );
	moveForward( fd * 0.1f );
	moveRight( sd * 0.1f );
	moveUp( ud * 0.2f );
}

void FpsCamera::setLookAt( vec3 lookAt ){
	forward = normalize(lookAt - pos);
	right = cross( forward, vec3(0,1,0) );
}

void FpsCamera::rotateY( float angle ){
	rotate( vec3(0,1,0), angle );
}

} /* namespace OGLPool */
