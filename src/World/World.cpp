/*
 * World.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "World.h"
#include <iostream>
#include <algorithm>
#include <SFML/OpenGL.hpp>
using namespace std;

namespace OGLPool {

World::World() {
	camera = 0;
}

World::~World() {
	clearBodies();

	if( camera ) delete camera;
}

void World::render(){
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	if( camera ) camera->applyView();

	for( auto it = bodies.begin(); it != bodies.end(); it++ ){
		RigidBody* e = (*it);
		e->render();
	}
}

void World::update( float dt ){
	if( camera ) camera->update( dt );

	for( auto it = bodies.begin(); it != bodies.end(); it++ ){
		RigidBody* e = (*it);
		e->update( dt );
	}
}

void World::addBody( RigidBody* e ){
	bodies.push_back( e );
}

void World::clearBodies(){
	bodies.erase( remove_if( bodies.begin(), bodies.end(),
		[](RigidBody* element) -> bool {
			delete element;
			return true;
		}
	), bodies.end() );
}

} /* namespace OGLPool */
