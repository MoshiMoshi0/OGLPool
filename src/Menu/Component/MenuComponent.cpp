/*
 * MenuComponent.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "MenuComponent.h"

namespace OGLPool {

MenuComponent::MenuComponent( Menu* menu ){
	this->menu = menu;
	this->window = menu->getWindow();
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
