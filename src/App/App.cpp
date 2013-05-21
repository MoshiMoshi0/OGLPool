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
#include <Util/Debug/Debug.h>
using namespace std;

namespace OGLPool {
App::App( int width, int height ) {
	this->width = width;
	this->height = height;

	camera = 0;
	window = 0;
	world = 0;
	initialized = false;
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
	const float dt = 1/60.f;
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

	world->render();
	Debug::render();
	window->display();
}

void App::update( float dt ) {
	IO::Input::update();
	camera->update( dt );
	world->update( dt );
}

bool App::init() {
	srand( time(NULL) );

	window = new RenderWindow(VideoMode( width, height, 24), "OpenGL Pool" );
	window->setVerticalSyncEnabled( true );
	IO::Input::init( window );

	world = new World();

	camera = new FpsCamera( vec3(15, 15, 15) );
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

	IO::Input::setFocus( true );

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	Debug::init();
	initialized = true;
	return true;
}

void App::deinit() {
	if( window ) delete window;
	if( world ) delete world;
	if( camera ) delete camera;

	Debug::deinit();
	initialized = false;
}

void App::poolEvents() {
	Event e;

	while (window->pollEvent(e)) {
		if (e.type == Event::Closed) {
			window->close();
		}else if(e.type == Event::LostFocus){
			IO::Input::setFocus( false );
		}else if(e.type == Event::GainedFocus){
			IO::Input::setFocus( true );
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
