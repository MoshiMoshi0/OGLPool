/*
 * App.cpp
 *
 *  Created on: Feb 29, 2012
 *      Author: _CORE7
 */

#include "App.h"
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <time.h>
#include <App/Input.h>
#include <Entity/Sphere.h>
using namespace std;

namespace OGLPool {
App::App( int width, int height ) {
	this->width = width;
	this->height = height;
}

App::~App() {
	if( initialized ) deinit();
}

int App::start() {
	if (!init()) {
		deinit();
		return 1;
	}

	run();
	deinit();
	return 0;
}

void App::run() {
	//@TODO variable time step
	const float dt = 0.15f;

	while (window->isOpen()) {
		poolEvents();
		update( dt );
		draw();
	}
}

void App::draw() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	window->clear();
	camera->applyView();

	glColor3f(1,1,1);
	glBegin(GL_QUADS);
		glVertex3f( -5, 0, 5 );
		glVertex3f( 5, 0, 5 );
		glVertex3f( 5, 0, -5 );
		glVertex3f( -5, 0, -5 );
	glEnd();

	glBegin(GL_LINES);
		glColor3f( 1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(10,0,0);

		glColor3f( 0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,10,0);

		glColor3f( 0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,10);
	glEnd();

	world->render();

	window->display();
}

void App::update( float dt ) {
	if( hasFocus ) IO::Input::update();
	camera->update( dt );
	world->update( dt );
}

bool App::init() {
	srand( time(NULL) );

	window = new RenderWindow(VideoMode( width, height, 24), "OpenGL Pool" );
	window->setVerticalSyncEnabled( true );
	IO::Input::init( window );

	world = new World();
	world->addEntity( new Sphere( 5, vec3( 0, 10, 0 ) ) );

	camera = new FpsCamera( vec3(5, 5, 5) );
	camera->setLookAt( vec3() );

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glLineWidth( 2 );
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, (float)width/height, 0.001f, 1000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	hasFocus = false;
	initialized = true;
	return true;
}

void App::deinit() {
	if( window ) delete window;
	if( world ) delete world;
	if( camera ) delete camera;
	initialized = false;
}

void App::poolEvents() {
	Event e;

	while (window->pollEvent(e)) {
		if (e.type == Event::Closed) {
			window->close();
		}else if(e.type == Event::LostFocus){
			hasFocus = false;
		}else if(e.type == Event::GainedFocus){
			hasFocus = true;
		}else if (e.type == Event::Resized){
			float w = e.size.width; float h = e.size.height;
			glViewport( 0,0,w,h );
			gluPerspective(90, w/h, 0.001f, 1000.0f);
		}
	}

	if( IO::Input::isKeyPressed( IO::Input::Escape ) ){
		window->close();
	}
}
} /* namespace OGLPool */
