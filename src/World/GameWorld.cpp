/*
 * GameWorld.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "GameWorld.h"
#include <RigidBody/Plane.h>
#include <RigidBody/Sphere.h>
#include <World/Camera/FpsCamera.h>
#include <Shape/Polygon/RandomPolygon.h>
#include <iostream>
#include <App/Input.h>
using namespace std;

namespace OGLPool {

GameWorld::GameWorld() : ct( CueTable( RandomPolygon( 10, 20 ), vector<vec2>() ) ) {
	addBody( new Plane( vec3(0,1,0), vec3() ) );
	addBody( new Plane( vec3(-1,0,0), vec3(50,50,0) ) );
	addBody( new Plane( vec3(1,0,0), vec3(-50,50,0) ) );
	addBody( new Plane( vec3(0,0,-1), vec3(0,50,50) ) );
	addBody( new Plane( vec3(0,0,1), vec3(0,50,-50) ) );
	addBody( ct.tableMesh );

	gravity = vec3(0,-10,0);

	camera = new FpsCamera( vec3(15, 15, 15) );
	camera->setLookAt( vec3() );
}

GameWorld::~GameWorld() {}

void GameWorld::render(){
	World::render();
}

void GameWorld::update( float dt ){
	World::update( dt );

	for( auto it = bodies.begin(); it != bodies.end(); it++ ){
		RigidBody* e = (*it);
		e->applyForce( gravity * e->mass );
	}

	//physics.processBodies( bodies, dt );

	if( IO::Input::onKeyPressed( IO::Input::R ) ){
		Sphere* s = new Sphere( 5, vec3( 0, 30, 0 ) );
		s->setLinVel( vec3(0.1, 0.1, 15) );
		s->setAngVel( vec3(20.8, 1, 0) );
		addBody( s );
	}
}

} /* namespace OGLPool */
