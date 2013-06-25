/*
 * SliderComponent.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef SLIDERCOMPONENT_H_
#define SLIDERCOMPONENT_H_

#include "MenuComponent.h"
#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {

class SliderComponent : public MenuComponent {
public:
	SliderComponent( vec2 pos, Menu* menu, RenderWindow* window );
	virtual ~SliderComponent();

	void render();
	void update( float dt );

	vec2 pos;
	float length;
	bool isGrabbed;
	RectangleShape sliderCenter;
};

} /* namespace OGLPool */
#endif /* SLIDERCOMPONENT_H_ */
