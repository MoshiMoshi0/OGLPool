/*
 * FpsCamera.cpp
 *
 *  Created on: 16-03-2013
 *      Author: _CORE7
 */

#include "FpsCamera.h"
#include <App/Input.h>

using namespace OGLPool::IO;
namespace OGLPool {

FpsCamera::FpsCamera() : Camera() {}
FpsCamera::~FpsCamera() {}

void FpsCamera::update( float dt ){
	rotateX( IO::Input::getMouseDX() );
	rotateY( IO::Input::getMouseDY() );

	int fd = Input::isKeyPressed( IO::Input::W ) - Input::isKeyPressed( IO::Input::S );
	int sd = Input::isKeyPressed( IO::Input::A ) - Input::isKeyPressed( IO::Input::D );
	int ud = Input::isKeyPressed( IO::Input::Space ) - Input::isKeyPressed( IO::Input::LControl );
	moveForward( fd * 0.1f );
	moveRight( sd * 0.1f );
	moveUp( ud * 0.2f );
}

void FpsCamera::rotateX( float angle ){
	rotate( vec3(0,0,1), angle );
}

} /* namespace OGLPool */
