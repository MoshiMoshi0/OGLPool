/*
 * SliderComponent.cpp
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#include "SliderComponent.h"
#include <App/Input.h>

namespace OGLPool {

SliderComponent::SliderComponent( vec2 pos, Menu* menu, RenderWindow* window ) : MenuComponent( menu, window ) {
	this->pos = pos;
	this->length = 100;
	sliderCenter = RectangleShape( Vector2f(20,15) );
	sliderCenter.setPosition( pos.x, pos.y );
}

SliderComponent::~SliderComponent(){}

void SliderComponent::render(){
	window->draw( sliderCenter );
}

void SliderComponent::update( float dt ){
	ivec2 mousePos = IO::Input::getMousePosition();
	if( IO::Input::onButtonPressed( IO::Input::Mouse::Left ) ){
		if( sliderCenter.getGlobalBounds().contains( mousePos.x, mousePos.y ) ){
			isGrabbed = true;
		}
	}
	if( IO::Input::onButtonReleased( IO::Input::Mouse::Left ) ){
		isGrabbed = false;
	}

	if( isGrabbed ){
		float ee = (length - sliderCenter.getLocalBounds().width) / 2.0f;
		float xx = clamp<float>(mousePos.x, pos.x - ee, pos.x + ee );
		sliderCenter.setPosition( xx, pos.y );
	}
}

} /* namespace OGLPool */
