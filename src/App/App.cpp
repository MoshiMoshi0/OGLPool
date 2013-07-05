/*
 * App.cpp
 *
 *  Created on: Feb 29, 2012
 *      Author: _CORE7
 */

#include "App.h"
#include "Time.h"
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <App/Time.h>
#include <SFML/System/Time.hpp>
#include <App/Input.h>
#include <Util/UDebug/Debug.h>
#include <windows.h>
#include <Menu/RandomPolygonMenu.h>
#include <World/GameWorld.h>
using namespace std;

namespace OGLPool {

App::App( int width, int height ) {
	this->width = width;
	this->height = height;

	window = 0;
	menu = nextMenu = 0;
	world = nextWorld = 0;
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

void App::setMenu( Menu* menu ){
	this->nextMenu = menu;
	changeMenu = true;
}

void App::setWorld( World* world ){
	this->nextWorld = world;
	changeWorld = true;
}

RenderWindow* App::getWindow(){
	return window;
}

void App::run() {
	Clock clock;

	sf::Time lastTime = clock.getElapsedTime();
	double accum = 0.0;
	while (window->isOpen()) {
		sf::Time currentTime = clock.getElapsedTime();
		sf::Time frameTime = currentTime - lastTime;
		lastTime = currentTime;

		accum += frameTime.asMilliseconds() / 1000.0f;
		if( accum > 0.25f ) accum = 0.25f;

		poolEvents();
		while( accum > Time::DELTA_TIME ){
			update( Time::DELTA_TIME );
			accum -= Time::DELTA_TIME;
		}
		draw();
	}
}

void App::draw() {
	window->clear();

	if( menu ){
		menu->render();
	}else{
		world->render();
	}

	Debug::render();
	window->display();
}

void App::update( float dt ) {
	IO::Input::update();
	if( IO::Input::getFocus() ){
		if( menu ){
			menu->update(dt);
		}else{
			world->update( dt );
		}
	}
}

bool App::init() {
	srand( time(NULL) );

	window = new RenderWindow(VideoMode( width, height, 24), "OpenGL Pool" );
	window->setVerticalSyncEnabled( true );
	IO::Input::init( window );
	IO::Input::setFocus( false );

	setMenu( new RandomPolygonMenu(this) );
	setWorld( 0 );
	//setWorld( new GameWorld( CueTable() ) );

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
	if( menu ) delete menu;

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
		}else if(e.type == Event::MouseButtonReleased){
			if(!IO::Input::getFocus()){
				IO::Input::setFocus( true );
			}
		}else if (e.type == Event::Resized){
			float w = e.size.width; float h = e.size.height;
			glViewport( 0,0,w,h );
			gluPerspective(90, w/h, 0.001f, 1000.0f);
		}
	}

	if( IO::Input::isKeyPressed( IO::Input::Escape ) ){
		window->close();
	}

	if( changeMenu ){
		if( menu ) delete menu;
		menu = nextMenu;
		changeMenu = false;
	}
	if( changeWorld ){
		if( world ) delete world;
		world = nextWorld;
		changeWorld = false;
	}
}
} /* namespace OGLPool */
