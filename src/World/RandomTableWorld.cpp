/*
 * RandomTableWorld.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "RandomTableWorld.h"
#include <glm/gtc/quaternion.hpp>
#include <Shape/Polygon/RandomPolygon.h>
#include <World/Camera/CinematicCamera.h>

namespace OGLPool {

RandomTableWorld::RandomTableWorld() {
	camera = new CinematicCamera( vec3(30,100,0) );
}

RandomTableWorld::~RandomTableWorld(){}

void RandomTableWorld::update( float dt ){
	camera->setLookAt( vec3() );

	vec3 newPos = camera->pos * angleAxis( 10 * dt, vec3(0,1,0) );
	camera->pos = newPos;
}

} /* namespace OGLPool */
