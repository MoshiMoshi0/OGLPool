/*
 * World.cpp
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#include "World.h"
#include <RigidBody/Sphere.h>
#include <RigidBody/Plane.h>
#include <RigidBody/Mesh.h>
#include <iostream>
#include <App/Input.h>
using namespace std;

namespace OGLPool {

World::World() {
	addEntity( new Plane( vec3(0,1,0), vec3() ) );
	addEntity( new Plane( vec3(-1,0,0), vec3(50,50,0) ) );
	addEntity( new Plane( vec3(1,0,0), vec3(-50,50,0) ) );
	addEntity( new Plane( vec3(0,0,-1), vec3(0,50,50) ) );
	addEntity( new Plane( vec3(0,0,1), vec3(0,50,-50) ) );

	gravity = vec3(0,-10,0);

	/*Mesh* mesh = new Mesh();

	for( int i = -5; i < 5; i++ ){
		for( int j = -5; j < 5; j++ ){
			float x0 = i * 10;
			float y0 = j * 10;
			float x1 = (i+1) * 10;
			float y1 = (j+1) * 10;
			vec3 v0 = vec3( x0, 0, y0 );
			vec3 v1 = vec3( x1, 0, y0 );
			vec3 v2 = vec3( x0, 0, y1 );
			vec3 v3 = vec3( x1, 0, y1 );

			mesh->beginTriangle();
				mesh->vertex( v2 );
				mesh->vertex( v1 );
				mesh->vertex( v0 );

				mesh->color( vec3(1.0, 0.0, 0.0) );
				mesh->color( vec3(0.0, 1.0, 0.0) );
				mesh->color( vec3(0.0, 0.0, 1.0) );
				mesh->calculateNormals();
			mesh->endTriangle();

			mesh->beginTriangle();
				mesh->vertex( v2 );
				mesh->vertex( v3 );
				mesh->vertex( v1 );

				mesh->color( vec3(1.0, 0.0, 0.0) );
				mesh->color( vec3(0.0, 1.0, 0.0) );
				mesh->color( vec3(0.0, 0.0, 1.0) );
				mesh->calculateNormals();
			mesh->endTriangle();
		}
	}

	mesh->build();
	addEntity( mesh );*/
}

World::~World() {
	bodies.erase( remove_if( bodies.begin(), bodies.end(),
		[](RigidBody* element) -> bool {
			delete element;
			return true;
		}
	), bodies.end() );
}

void World::render(){
	for( auto it = bodies.begin(); it != bodies.end(); it++ ){
		RigidBody* e = (*it);
		e->render();
	}
}

void World::update( float dt ){
	for( auto it = bodies.begin(); it != bodies.end(); it++ ){
		RigidBody* e = (*it);
		e->update( dt );

		e->applyForce( gravity * e->mass );
	}

	physics.processBodies( bodies, dt );

	if( IO::Input::onKeyPressed( IO::Input::R ) ){
		Sphere* s = new Sphere( 5, vec3( 0, 30, 0 ) );
		s->setLinVel( vec3(0.1, 0.1, 15) );
		s->setAngVel( vec3(20.8, 1, 0) );
		addEntity( s );
	}
}

void World::addEntity( RigidBody* e ){
	bodies.push_back( e );
}

} /* namespace OGLPool */
