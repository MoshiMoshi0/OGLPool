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
	this->halfLength = length / 2.0f;
	sliderCenter = RectangleShape( Vector2f(20,15) );
	sliderCenter.setPosition( pos.x, pos.y );
}

SliderComponent::~SliderComponent(){}

void SliderComponent::render(){
	window->draw( sliderCenter );
}

void SliderComponent::update( float dt ){
	MenuComponent::update( dt );
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
		setValue( clamp<float>(mousePos.x - pos.x + halfLength, 0, length ) );
	}
}

void SliderComponent::setValue( float value ){
	this->value = value;
	sliderCenter.setPosition( value + pos.x - halfLength - sliderCenter.getLocalBounds().width / 2.0f, pos.y );
	setChanged( true );
}

float SliderComponent::getPercent(){
	return value / length;
}

float SliderComponent::getValue(){
	return value;
}

float SliderComponent::getValue( float min, float max ){
	return min + (max - min) * getPercent();
}

} /* namespace OGLPool */
