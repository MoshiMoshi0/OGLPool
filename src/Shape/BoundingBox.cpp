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
#include <Util/UDebug/Debug.h>

namespace OGLPool {

BoundingBox::BoundingBox() : min(vec3()), max(vec3()), minDyn(vec3()), maxDyn(vec3()), pos(vec3()), skin(0) {}
BoundingBox::~BoundingBox(){}

void BoundingBox::render() const{
	vec3 c = pos;
	vec3 e = (max - min) / 2.0f;
	float ex = e.x;
	float ey = e.y;
	float ez = e.z;

	auto debugDraw = Debug::getDebugDraw();
	debugDraw->setColor( 1,0,0 );
	debugDraw->drawLine( c + vec3(ex,ey,ez), c + vec3(-ex,ey,ez) );
	debugDraw->drawLine( c + vec3(-ex,ey,ez), c + vec3(-ex,-ey,ez) );
	debugDraw->drawLine( c + vec3(-ex,-ey,ez), c + vec3(ex,-ey,ez) );
	debugDraw->drawLine( c + vec3(ex,-ey,ez), c + vec3(ex,ey,ez) );

	debugDraw->drawLine( c + vec3(ex,ey,-ez), c + vec3(-ex,ey,-ez) );
	debugDraw->drawLine( c + vec3(-ex,ey,-ez), c + vec3(-ex,-ey,-ez) );
	debugDraw->drawLine( c + vec3(-ex,-ey,-ez), c + vec3(ex,-ey,-ez) );
	debugDraw->drawLine( c + vec3(ex,-ey,-ez), c + vec3(ex,ey,-ez) );
}

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

	bb.pos = ( bb.max + bb.min ) / 2.0f;
	bb.min -= bb.pos;
	bb.max -= bb.pos;

	if( addSkin ){
		bb.min -= vec3(1,1,1) * skin;
		bb.max += vec3(1,1,1) * skin;
	}

	bb.skin = skin;
	return bb;
}

BoundingBox BoundingBox::get( vec3 min, vec3 max, float skin, bool addSkin ){
	BoundingBox bb;

	bb.min = min;
	bb.max = max;

	if( addSkin ){
		bb.min -= vec3(1,1,1) * skin;
		bb.max += vec3(1,1,1) * skin;
	}

	bb.skin = skin;
	return bb;
}


} /* namespace OGLPool */
