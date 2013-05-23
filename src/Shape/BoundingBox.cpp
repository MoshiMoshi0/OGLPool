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
#include <limits>

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

BoundingBox BoundingBox::get( vector< vec3 > points, float skin, bool addSkin ){
	BoundingBox bb;
	bb.min = vec3(numeric_limits<float>::max(), numeric_limits<float>::max(), numeric_limits<float>::max());
	bb.max = vec3(-numeric_limits<float>::max(), -numeric_limits<float>::max(), -numeric_limits<float>::max());

	for( auto& p : points ){
		for( int i = 0; i < 3; i++ ){
			if( p[i] < bb.min[i] ) bb.min[i] = p[i];
			if( p[i] > bb.max[i] ) bb.max[i] = p[i];
		}
	}

	if( addSkin && skin != 0 ){
		bb.min += bb.min * skin / length( bb.min );
		bb.max += bb.min * skin / length( bb.max );
	}

	bb.skin = skin;
	return bb;
}

BoundingBox BoundingBox::get( vec3 min, vec3 max, float skin, bool addSkin ){
	BoundingBox bb;

	bb.min = min;
	bb.max = max;

	if( addSkin && skin != 0 ){
		bb.min += bb.min * skin / length( min );
		bb.max += bb.min * skin / length( max );
	}

	bb.skin = skin;
	return bb;
}


} /* namespace OGLPool */
