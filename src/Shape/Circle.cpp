/*
 * Circle.cpp
 *
 *  Created on: 23-03-2013
 *      Author: _CORE7
 */

#include "Circle.h"

namespace OGLPool {

Circle::Circle() {}
Circle::Circle( vec2 p, float r ) : center(p), radius(r) {}

Circle::~Circle() {}

bool Circle::inside( vec2 p ){
	return dot( center-p, center-p ) <= radius * radius;
}

Circle Circle::circumCircle(vec2 p1, vec2 p2, vec2 p3) {
	Circle circle;
	vec2 v21 = p2 - p1;
	vec2 v31 = p3 - p1;
	vec2 v32 = p3 - p2;
	float cp = v21.x * v31.y - v21.y * v31.x;

	if (cp != 0.0){
		float den = (2.0f * cp);
		float p1Sq = dot( p1, p1 );
		float p2Sq = dot( p2, p2 );
		float p3Sq = dot( p3, p3 );

		float cx = (-p1Sq*v32.y + p2Sq*v31.y - p3Sq*v21.y) / den;
		float cy = (p1Sq*v32.x - p2Sq*v31.x + p3Sq*v21.x) / den;

		circle.center = vec2(cx, cy);
	}

	circle.radius = distance( circle.center, p1 );
	return circle;
}

} /* namespace OGLPool */
