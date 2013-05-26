/*
 * Circle.h
 *
 *  Created on: 23-03-2013
 *      Author: _CORE7
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <glm/glm.hpp>
#include <Shape/Shape.h>
using namespace glm;

namespace OGLPool {

class Circle : public Shape {
public:
	static Circle circumCircle(vec2 p1, vec2 p2, vec2 p3);

	Circle();
	Circle( vec2 p, float r );
	virtual ~Circle();

	vec2 c;
	float r;

	bool inside( vec2 p );
};

} /* namespace OGLPool */
#endif /* CIRCLE_H_ */
