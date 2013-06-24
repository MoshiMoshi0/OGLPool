/*
 * TestMenu.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "TestMenu.h"

namespace OGLPool {

TestMenu::TestMenu( RenderWindow* window ) : Menu( window ){}

TestMenu::~TestMenu(){}

void TestMenu::render(){
	CircleShape circle;
	circle.setRadius( 10 );
	circle.setPosition( 200, 200 );

	window->draw( circle );
}

void TestMenu::update(float dt){}

} /* namespace OGLPool */
