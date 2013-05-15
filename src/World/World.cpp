/*
 * World.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "World.h"
#include <algorithm>
#include <Entity/Sphere.h>
#include <Physics/Physics.h>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <App/Input.h>
#include <Physics/ConstraintSolver/ImpulseConstraintSolver.h>
using namespace std;

namespace OGLPool {

World::World() {
	addEntity( new Plane( vec3(0,1,0), vec3() ) );
	addEntity( new Plane( vec3(-1,0,0), vec3(-25,25,0) ) );
	addEntity( new Plane( vec3(1,0,0), vec3(25,25,0) ) );
	addEntity( new Plane( vec3(0,0,-1), vec3(0,25,25) ) );
	addEntity( new Plane( vec3(0,0,1), vec3(0,25,-25) ) );

	gravity = vec3(0,-10,0);
}

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

		e->applyForce( gravity );
	}

	vector< ContactInfo* > infos;
	for( auto it0 = entities.begin(); it0 != entities.end() - 1; it0++ ){
		for( auto it1 = it0 + 1; it1 != entities.end(); it1++ ){
			Entity* e0 = (*it0);
			Entity* e1 = (*it1);

			ContactInfo* info = new ContactInfo( dt );
			if( Physics::checkCollision( e0, e1, info ) ){
				infos.push_back(info);
			}else{
				delete info;
			}
		}
	}

	if( infos.size() != 0 ){
		Physics::ImpulseConstraintSolver solver;
		solver.solveGroup( entities, infos );
	}

	if( IO::Input::onKeyPressed( IO::Input::R ) ){
		Sphere* s = new Sphere( 5, vec3( 0, 10, 0 ) );
		//s->setVel( vec3(0.1, 0.1, 1) );
		s->setAngVel( vec3(-2.8, 1, 0) );
		addEntity( s );
	}
}

void World::addEntity( Entity* e ){
	entities.push_back( e );
}

} /* namespace OGLPool */
