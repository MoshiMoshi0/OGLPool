/*
 * LabelComponent.h
 *
 *  Created on: 25-06-2013
 *      Author: _CORE7
 */

#ifndef LABELCOMPONENT_H_
#define LABELCOMPONENT_H_

#include "MenuComponent.h"
#include <string>
#include <glm/glm.hpp>
using namespace glm;
using namespace std;

namespace OGLPool {

class LabelComponent : public MenuComponent {
public:
	LabelComponent( string text, vec2 pos, Menu* menu, RenderWindow* window );
	virtual ~LabelComponent();

	void render();
	void setText( string str );

	vec2 pos;
	string text;

	sf::Font font;
	sf::Text sfText;
};

} /* namespace OGLPool */
#endif /* LABELCOMPONENT_H_ */
