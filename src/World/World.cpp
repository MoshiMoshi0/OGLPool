/*
 * World.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "World.h"
#include <algorithm>

#include <SFML/OpenGL.hpp>
#include <Misc/Delaunay.h>
#include <glm/gtc/random.hpp>
#include <vector>
#include <App/Input.h>
#include <iostream>
using namespace std;

namespace OGLPool {

World::World() : sp(SmoothRandomPolygon()) {}

World::~World() {
	entities.erase( remove_if( entities.begin(), entities.end(),
		[](Entity* element) -> bool {
			delete element;
			return true;
		}
	), entities.end() );
}

void World::render(){
	for( auto it = entities.begin(); it != entities.end(); it++ ){
		Entity* e = (*it);
		e->render();
	}

	sp.draw();
}

void World::update( float dt ){
	for( auto it = entities.begin(); it != entities.end(); it++ ){
		Entity* e = (*it);
		e->update( dt );
	}
	if( IO::Input::onKeyPressed( IO::Input::R )){
		sp = SmoothRandomPolygon();
	}
}

void World::addEntity( Entity* e ){
	entities.push_back( e );
}

} /* namespace OGLPool */
