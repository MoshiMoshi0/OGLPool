/*
 * TestMenu.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "TestMenu.h"
#include "Component/SliderComponent.h"

namespace OGLPool {

TestMenu::TestMenu( RenderWindow* window ) : Menu( window ){
	addComponent( new SliderComponent( vec2( 200, 20 ), this, window ) );
}

TestMenu::~TestMenu(){}

} /* namespace OGLPool */
