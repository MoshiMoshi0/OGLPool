/*
 * MenuComponent.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "MenuComponent.h"

namespace OGLPool {

MenuComponent::MenuComponent( Menu* menu, RenderWindow* window ){
	this->menu = menu;
	this->window = window;
	this->changed = false;
}

MenuComponent::~MenuComponent(){}

void MenuComponent::render(){}

void MenuComponent::update( float dt ){
	changed = false;
}

bool MenuComponent::isChanged(){
	return changed;
}

void MenuComponent::setChanged( bool changed ){
	this->changed = changed;
}

} /* namespace OGLPool */
