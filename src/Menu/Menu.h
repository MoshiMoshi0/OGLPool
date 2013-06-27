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
#include <App/app.h>
using namespace std;
using namespace sf;

namespace OGLPool {

class App;
class MenuComponent;
class Menu {
public:
	Menu( App* app );
	virtual ~Menu();

	virtual void render();
	virtual void update( float dt );
	virtual void addComponent( MenuComponent* component );

	virtual bool isChanged();
	RenderWindow* getWindow();

	vector< MenuComponent* > components;
	App* app;
	RenderWindow* window;
};

} /* namespace OGLPool */
#endif /* MENU_H_ */
