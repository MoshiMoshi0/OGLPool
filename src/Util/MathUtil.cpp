/*
 * MathUtil.cpp
 *
 *  Created on: 17-05-2013
 *      Author: _CORE7
 */

#include "MathUtil.h"

namespace OGLPool {

void planeSpace( const vec3& n, vec3& t0, vec3& t1 ){
	if( fabs( n.z ) > sqrtf( 1/2 ) ){
		float a = n.y * n.y + n.z * n.z;
		float k = 1.0f/sqrt( a );

		t0 = vec3(0, -n.z * k, n.y * k);
		t1 = vec3(a*k, -n.x * t0.z, n.x * t0.y);
	}else{
		float a = n.x * n.x + n.y * n.y;
		float k = 1.0f/sqrt( a );

		t0 = vec3(-n.y*k, n.x * k, 0);
		t1 = vec3(-n.z * t0.y, n.z * t0.x, a*k);
	}
}

} /* namespace OGLPool */
