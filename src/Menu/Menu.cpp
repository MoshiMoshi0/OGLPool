/*
 * Menu.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "Menu.h"

namespace OGLPool {

Menu::Menu( App* app ){
	this->app = app;
	this->window = app->getWindow();
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

void Menu::addComponent( MenuComponent* component ){
	components.push_back( component );
}

bool Menu::isChanged(){
	for( auto& component : components ){
		if( component->isChanged() ) return true;
	}

	return false;
}

RenderWindow* Menu::getWindow(){
	return window;
}

} /* namespace OGLPool */
