/*
 * ButtonComponent.cpp
 *
 *  Created on: 27-06-2013
 *      Author: _CORE7
 */

#include "ButtonComponent.h"
#include <App/Input.h>

namespace OGLPool {

ButtonComponent::ButtonComponent( string text, vec2 pos, function< void() > callback, Menu* menu ) : MenuComponent( menu ) {
	this->pos = pos;
	this->callback = callback;

	if( !font.loadFromFile("arial.ttf") )
		assert(0);

	sfText.setScale( 0.5f, 0.5f );
	sfText.setFont( font );
	sfText.setString( text );
	sfText.setPosition( pos.x, pos.y );
	sfText.setColor( sf::Color::White );

	rect.setSize( Vector2f( 50,20 ) );
	rect.setPosition( pos.x, pos.y );
	rect.setFillColor( sf::Color(20,20,20) );
	rect.setOutlineColor( sf::Color::White );
	rect.setOutlineThickness( 2.0f );
}

ButtonComponent::~ButtonComponent(){}

void ButtonComponent::render(){


	window->draw( rect );
	window->draw( sfText );
}

void ButtonComponent::update( float dt ){
	MenuComponent::update( dt );

	ivec2 mousePos = IO::Input::getMousePosition();
	if( IO::Input::onButtonReleased( IO::Input::Mouse::Left ) ){
		if( rect.getGlobalBounds().contains( mousePos.x, mousePos.y ) ){
			callback();
		}
	}
}

} /* namespace OGLPool */
