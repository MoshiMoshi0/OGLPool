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

vector< vec2 > points;
Delaunay d;
World::World() {
	for( int i = 0; i < 5; i++ ){
		points.push_back( linearRand( vec2(-1.0f,-1.0f ), vec2(1.0f,1.0f) ) * 20.0f );
	}

	d.triangulate( points );
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

	auto edges = d.getEdges();
	auto tris = d.getTriangles();

	glBegin( GL_LINES );
	for( int i = 0; i < edges.size(); i++ ){
		glVertex3f( edges[i][0].x, 10, edges[i][0].y );
		glVertex3f( edges[i][1].x, 10, edges[i][1].y );
	}
	glEnd();

	glBegin( GL_TRIANGLES );
	for( int i = 0; i < tris.size(); i++ ){
		glColor3f( (255 - i * 30) / 255.0, 255 / 255.0, i * 30 / 255.0 );
		glVertex3f( tris[i][0].x, 9.5, tris[i][0].y );
		glVertex3f( tris[i][1].x, 9.5, tris[i][1].y );
		glVertex3f( tris[i][2].x, 9.5, tris[i][2].y );
	}
	glEnd();
}

void World::update( float dt ){
	for( auto it = entities.begin(); it != entities.end(); it++ ){
		Entity* e = (*it);
		e->update( dt );
	}
	if( IO::Input::isKeyPressed( IO::Input::R )){
		points.clear();
		for( int i = 0; i < 5; i++ ){
			points.push_back( linearRand( vec2(-1.0f,-1.0f ), vec2(1.0f,1.0f) ) * 20.0f );
		}
		d = Delaunay();
		d.triangulate( points );
	}
}

void World::addEntity( Entity* e ){
	entities.push_back( e );
}

} /* namespace OGLPool */
