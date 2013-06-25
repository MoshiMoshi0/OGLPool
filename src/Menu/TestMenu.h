/*
 * TestMenu.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef TESTMENU_H_
#define TESTMENU_H_

#include "Menu.h"

namespace OGLPool {

class TestMenu : public Menu {
public:
	TestMenu( RenderWindow* window );
	virtual ~TestMenu();
};

} /* namespace OGLPool */
#endif /* TESTMENU_H_ */
