/*
 * World.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "World.h"
#include <algorithm>

using namespace std;

namespace OGLPool {

World::World() {}

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
}

void World::update( float dt ){
	for( auto it = entities.begin(); it != entities.end(); it++ ){
		Entity* e = (*it);
		e->update( dt );
	}
}

void World::addEntity( Entity* e ){
	entities.push_back( e );
}

} /* namespace OGLPool */
