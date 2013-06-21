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

template< class T >
BoundingBox<T>::BoundingBox() : min(T()), max(T()), minDyn(T()), maxDyn(T()), pos(T()), skin(0) {}
template< class T >
BoundingBox<T>::~BoundingBox(){}

template<>
void BoundingBox<vec3>::render() const{
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

template<>
void BoundingBox<vec2>::render() const{
	vec3 c = vec3( pos.x, 0, pos.y );
	vec2 e = (max - min) / 2.0f;
	float ex = e.x;
	float ey = e.y;

	auto debugDraw = Debug::getDebugDraw();
	debugDraw->setColor( 1,0,0 );
	debugDraw->drawLine( c + vec3(ex,0,ey), c + vec3(-ex,0,ey) );
	debugDraw->drawLine( c + vec3(-ex,0,ey), c + vec3(-ex,0,-ey) );
	debugDraw->drawLine( c + vec3(-ex,0,-ey), c + vec3(ex,0,-ey) );
	debugDraw->drawLine( c + vec3(ex,0,-ey), c + vec3(ex,0,ey) );
}

template< class T >
bool BoundingBox<T>::intersects( const BoundingBox<T>& b0, const BoundingBox<T>& b1 ){
	static uint size = T().length();
	for( uint i = 0; i < size; i++ ) {
		float min0 = b0.pos[i] + b0.min[i] + b0.minDyn[i];
		float max0 = b0.pos[i] + b0.max[i] + b0.maxDyn[i];

		float min1 = b1.pos[i] + b1.min[i] + b1.minDyn[i];
		float max1 = b1.pos[i] + b1.max[i] + b1.maxDyn[i];

		if( !( min0 < max1 && max0 > min1 ) )
			return false;
	}

	return true;
}

template< class T >
BoundingBox<T> BoundingBox<T>::get( vector< T > points, float skin, bool addSkin ){
	BoundingBox<T> bb;

	bb.min = T(numeric_limits<float>::max());
	bb.max = T(-numeric_limits<float>::max());

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
		bb.min -= T(1) * skin;
		bb.max += T(1) * skin;
	}

	bb.skin = skin;
	return bb;
}

template< class T >
BoundingBox<T> BoundingBox<T>::get( T min, T max, float skin, bool addSkin ){
	BoundingBox<T> bb;

	bb.min = min;
	bb.max = max;

	if( addSkin ){
		bb.min -= T(1) * skin;
		bb.max += T(1) * skin;
	}

	bb.skin = skin;
	return bb;
}

template class BoundingBox< vec2 >;
template class BoundingBox< vec3 >;

} /* namespace OGLPool */
