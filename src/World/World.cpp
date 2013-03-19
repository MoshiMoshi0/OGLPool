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
using namespace std;

using namespace std;

namespace OGLPool {

World::World() {
	Sphere* s = new Sphere( 5, vec3( 3, 25, 0 ) );
	s->setVel( vec3( 0, -0.1, 0 ) );
	s->setAngVel( vec3(0,0.1,0) );
	addEntity( s );
	s = new Sphere( 5, vec3( 0, 10, 0 ) );
	s->setVel( vec3( 0.1, 0.4, 0 ) );
	s->setAngVel( vec3(1,0.5,0) );
	addEntity( s );

	plane = new Plane( vec3(0,1,0), vec3() );
	addEntity( plane );
	gravity = vec3(0,-1,0);
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

	Sphere* s0 = (Sphere*)entities.at( 0 );
	Sphere* s1 = (Sphere*)entities.at( 1 );

	ContactInfo info;
	if( Physics::sphereSphereTest( s0, s1, &info ) ){
		cout << info.getContactTime() << endl;
		Physics::response( s0, s1, &info );
	}

	info = ContactInfo();
	if( Physics::spherePlaneTest( s0, plane, &info) ){
		cout << "s0 " << info.getContactTime() << endl;
		Physics::response( s0, plane, &info );
	}

	info = ContactInfo();
	if( Physics::spherePlaneTest( s1, plane, &info) ){
		cout << "s1 " << info.getContactTime() << endl;
		Physics::response( s1, plane, &info );
	}
}

void World::addEntity( Entity* e ){
	entities.push_back( e );
}

} /* namespace OGLPool */
