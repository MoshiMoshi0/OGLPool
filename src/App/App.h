/*
 * App.h
 *
 *  Created on: Feb 29, 2012
 *      Author: _CORE7
 */

#ifndef APP_H_
#define APP_H_

#include <SFML/Graphics.hpp>
#include <World/Camera/FpsCamera.h>
#include <World/World.h>

using namespace sf;
namespace OGLPool {

class App{
public:
	App( int width, int height );
	virtual ~App();
	int start();

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
	Camera* camera;
	RenderWindow* window;
	bool initialized;
	bool hasFocus;
};

} /* namespace OGLPool */
#endif /* APP_H_ */
