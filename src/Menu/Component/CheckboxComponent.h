/*
 * CheckboxComponent.h
 *
 *  Created on: 27-06-2013
 *      Author: _CORE7
 */

#ifndef CHECKBOXCOMPONENT_H_
#define CHECKBOXCOMPONENT_H_

#include "MenuComponent.h"
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include <SFML/Graphics/Text.hpp>
using namespace glm;
using namespace std;

namespace OGLPool {

class CheckboxComponent : public MenuComponent {
public:
	CheckboxComponent( vector< string > strings, vec2 pos, Menu* menu );
	virtual ~CheckboxComponent();

	void render();
	void update( float dt );

	int getSelectedIndex();
	void setSelected( int i );
	string getSelected();

	vec2 pos;
	sf::Font font;
	int selected;
	vector< string > strings;
	vector< sf::Text > texts;
};

} /* namespace OGLPool */
#endif /* CHECKBOXCOMPONENT_H_ */
