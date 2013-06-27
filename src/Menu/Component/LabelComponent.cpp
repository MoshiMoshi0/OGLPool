/*
 * LabelComponent.cpp
 *
 *  Created on: 25-06-2013
 *      Author: _CORE7
 */

#include "LabelComponent.h"

namespace OGLPool {

LabelComponent::LabelComponent( string text, vec2 pos, Menu* menu ) : MenuComponent( menu ){
	this->text = text;
	this->pos = pos;

	if( !font.loadFromFile("arial.ttf") )
		assert(0);

	sfText.setFont( font );
	sfText.setString( text );
	sfText.setColor( sf::Color::White );
	sfText.setPosition( pos.x, pos.y );
	sfText.setScale( 0.5f, 0.5f );
}

LabelComponent::~LabelComponent(){}

void LabelComponent::render(){
	window->draw( sfText );
}

void LabelComponent::setText( string str ){
	text = str;
	sfText.setString( str );
}

} /* namespace OGLPool */
