/*
 * MenuComponent.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef MENUCOMPONENT_H_
#define MENUCOMPONENT_H_

#include "..\Menu.h"
#include <vector>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
using namespace std;
using namespace sf;

namespace OGLPool {

class Menu;
class MenuComponent {
public:
	MenuComponent( Menu* menu, RenderWindow* window );
	virtual ~MenuComponent();

	virtual void render();
	virtual void update( float dt );

	Menu* menu;
	RenderWindow* window;
};

} /* namespace OGLPool */
#endif /* MENUCOMPONENT_H_ */
