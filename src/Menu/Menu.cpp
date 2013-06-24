/*
 * Menu.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "Menu.h"

namespace OGLPool {

Menu::Menu( RenderWindow* window ){
	this->window = window;
}

Menu::~Menu(){
	components.erase( remove_if( components.begin(), components.end(),
		[](MenuComponent* element) -> bool {
			delete element;
			return true;
		}
	), components.end() );
}

void Menu::render(){
	for( auto& component : components ){
		component->render();
	}
}

void Menu::update( float dt ){
	for( auto& component : components ){
		component->update( dt );
	}
}

} /* namespace OGLPool */
