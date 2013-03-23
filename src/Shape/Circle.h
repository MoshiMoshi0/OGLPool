/*
 * Circle.h
 *
 *  Created on: 23-03-2013
 *      Author: _CORE7
 */

#ifndef CIRCLE_H_
#define CIRCLE_H_

#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {

class Circle {
public:
	Circle();
	virtual ~Circle();

	vec2 c;
	float r;

	bool inside( vec2 p ){
		return dot( c-p, c-p ) < r * r;
	}

	static Circle circumCircle(vec2 p1, vec2 p2, vec2 p3) {
		Circle circle;
		vec2 v21 = p2 - p1;
		vec2 v31 = p3 - p1;
		float cp = v21.x * v31.y - v21.y * v31.x;

		if (cp != 0.0){
			float p1Sq, p2Sq, p3Sq;
			float num, den;
			float cx, cy;

			p1Sq = p1.x * p1.x + p1.y * p1.y;
			p2Sq = p2.x * p2.x + p2.y * p2.y;
			p3Sq = p3.x * p3.x + p3.y * p3.y;
			num = p1Sq*(p2.y - p3.y) + p2Sq*(p3.y - p1.y) + p3Sq*(p1.y - p2.y);
			cx = num / (2.0f * cp);
			num = p1Sq*(p3.x - p2.x) + p2Sq*(p1.x - p3.x) + p3Sq*(p2.x - p1.x);
			cy = num / (2.0f * cp);

			circle.c = vec2(cx, cy);
		}

		circle.r = distance( circle.c, p1 );
		return circle;
    }
};

} /* namespace OGLPool */
#endif /* CIRCLE_H_ */
