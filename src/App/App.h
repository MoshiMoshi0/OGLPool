/*
 * App.h
 *
 *  Created on: Feb 29, 2012
 *      Author: _CORE7
 */

#ifndef APP_H_
#define APP_H_

#include <SFML/Graphics.hpp>
#include <World/World.h>
#include <Menu/Menu.h>

using namespace sf;
namespace OGLPool {

class Menu;
class App {
public:
	App( int width, int height );
	virtual ~App();
	int start();

	void setMenu( Menu* menu );
	void setWorld( World* world );
	RenderWindow* getWindow();

	int width;
	int height;
private:
	void poolEvents();
	void run();
	bool init();
	void deinit();
	void draw();
	void update( float dt );

	World* world;
	World* nextWorld;
	Menu* menu;
	Menu* nextMenu;
	RenderWindow* window;
	bool initialized, changeWorld, changeMenu;
};

} /* namespace OGLPool */
#endif /* APP_H_ */
