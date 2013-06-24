/*
 * Menu.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef MENU_H_
#define MENU_H_

#include <vector>
#include "Component/MenuComponent.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

namespace OGLPool {

class MenuComponent;
class Menu {
public:
	Menu( RenderWindow* window );
	virtual ~Menu();

	virtual void render();
	virtual void update( float dt );

	vector< MenuComponent* > components;
	RenderWindow* window;
};

} /* namespace OGLPool */
#endif /* MENU_H_ */
