/*
 * ButtonComponent.h
 *
 *  Created on: 27-06-2013
 *      Author: _CORE7
 */

#ifndef BUTTONCOMPONENT_H_
#define BUTTONCOMPONENT_H_

#include "MenuComponent.h"
#include <glm/glm.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
using namespace glm;

namespace OGLPool {

class ButtonComponent : public MenuComponent {
public:
	ButtonComponent( string text, vec2 pos, function< void() > callback, Menu* menu );
	virtual ~ButtonComponent();

	void render();
	void update( float dt );

	vec2 pos;
	sf::Font font;
	sf::Text sfText;
	RectangleShape rect;
	function< void() > callback;
};

} /* namespace OGLPool */
#endif /* BUTTONCOMPONENT_H_ */
