/*
 * CheckboxComponent.cpp
 *
 *  Created on: 27-06-2013
 *      Author: _CORE7
 */

#include "CheckboxComponent.h"
#include <App/Input.h>

namespace OGLPool {

CheckboxComponent::CheckboxComponent( vector< string > strings, vec2 pos, Menu* menu ) : MenuComponent( menu ) {
	this->pos = pos;

	if( !font.loadFromFile("arial.ttf") )
		assert(0);

	for( uint i = 0; i < strings.size(); i++ ){
		this->strings.push_back( strings[i] );

		sf::Text text;
		text.setString( strings[i] );
		//text.setScale( 0.5f, 0.5f );
		text.setFont( font );
		text.setColor( sf::Color::White );
		text.setPosition( pos.x, pos.y + i * 28 );
		this->texts.push_back( text );
	}

	selected = -1;
}

CheckboxComponent::~CheckboxComponent(){}

void CheckboxComponent::render(){
	for( uint i = 0; i < texts.size(); i++ ){
		auto sfT = texts[i];
		if( i == getSelectedIndex() ){
			sfT.setColor( sf::Color::Green );
		}else{
			sfT.setColor( sf::Color::White );
		}
		window->draw( sfT );
	}
}

void CheckboxComponent::update( float dt ){
	MenuComponent::update( dt );

	ivec2 mousePos = IO::Input::getMousePosition();
	if( IO::Input::onButtonReleased( IO::Input::Mouse::Left ) ){
		for( uint i = 0; i < texts.size(); i++ ){
			auto sfT = texts[i];
			if( sfT.getGlobalBounds().contains(mousePos.x, mousePos.y) ){
				selected = i;
				setChanged( true );
				break;
			}
		}
	}
}

int CheckboxComponent::getSelectedIndex(){
	if( selected < 0 || selected >= strings.size() ) return -1;
	return selected;
}

void CheckboxComponent::setSelected( int i ){
	selected = i;
	setChanged( true );
}

string CheckboxComponent::getSelected(){
	if( selected < 0 || selected >= strings.size() ) return "";
	return strings[ selected ];
}

} /* namespace OGLPool */
