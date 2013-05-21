/*
 * BoundingBox.cpp
 *
 *  Created on: 17-05-2013
 *      Author: _CORE7
 */

#include "BoundingBox.h"
#include <SFML/OpenGL.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OGLPool {

BoundingBox::BoundingBox() : min(vec3()), max(vec3()), minDyn(vec3()), maxDyn(vec3()), pos(vec3()), skin(0) {}
BoundingBox::~BoundingBox(){}

bool BoundingBox::intersects( const BoundingBox& b0, const BoundingBox& b1 ){
	for( uint i = 0; i < 3; i++ ) {
		float min0 = b0.pos[i] + b0.min[i] + b0.minDyn[i];
		float max0 = b0.pos[i] + b0.max[i] + b0.maxDyn[i];

		float min1 = b1.pos[i] + b1.min[i] + b1.minDyn[i];
		float max1 = b1.pos[i] + b1.max[i] + b1.maxDyn[i];

		if( !( min0 < max1 && max0 > min1 ) )
			return false;
	}

	return true;
}


} /* namespace OGLPool */
