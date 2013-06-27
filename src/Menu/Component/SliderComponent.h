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
	SliderComponent( vec2 pos, Menu* menu );
	virtual ~SliderComponent();

	void render();
	void update( float dt );

	float getPercent();
	float getValue();
	float getValue( float min, float max );
	void setValue( float value );

	float length;
	float halfLength;
	float value;

	bool isGrabbed;

	vec2 pos;
	RectangleShape sliderCenter;
};

} /* namespace OGLPool */
#endif /* SLIDERCOMPONENT_H_ */
