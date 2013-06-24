/*
 * MenuComponent.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef MENUCOMPONENT_H_
#define MENUCOMPONENT_H_

#include "..\Menu.h"

namespace OGLPool {

class Menu;
class MenuComponent {
public:
	MenuComponent( Menu* menu );
	virtual ~MenuComponent();

	virtual void render();
	virtual void update( float dt );

	Menu* menu;
};

} /* namespace OGLPool */
#endif /* MENUCOMPONENT_H_ */
