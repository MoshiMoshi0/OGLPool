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
#include "Input.h"
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

	window->display();
}

void App::update( float dt ) {
	IO::Input::update();
}

bool App::init() {
	srand( time(NULL) );

	window = new RenderWindow(VideoMode( width, height, 24), "OpenGL Pool" );
	window->setVerticalSyncEnabled( true );
	IO::Input::init( window );

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (float)width/height, 0.01, 1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    initialized = true;
	return true;
}

void App::deinit() {
	if( window ) delete window;
	initialized = false;
}

void App::poolEvents() {
	Event e;

	while (window->pollEvent(e)) {
		if (e.type == Event::Closed) {
			window->close();
		}
	}
}
} /* namespace OGLPool */
