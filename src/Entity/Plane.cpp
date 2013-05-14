/*
 * Plane.cpp
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#include "Plane.h"
#include <SFML/OpenGL.hpp>


namespace OGLPool {

Plane::Plane( vec3 normal, vec3 point ) : Entity( point ) {
	this->normal = normalize( normal );
	setInertia( mat3(0.0f) );
}

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

Plane::~Plane() {}

void Plane::render(){
	glColor3f( 1,1,1 );

	glPushMatrix();
	applyTransform();

	vec3 t0,t1;
	planeSpace( normal, t0, t1 );

	vec3 v0 = pos + t0 * 50.0f + t1 * 50.0f;
	vec3 v1 = pos + t0 * 50.0f - t1 * 50.0f;
	vec3 v2 = pos - t0 * 50.0f + t1 * 50.0f;
	vec3 v3 = pos - t0 * 50.0f - t1 * 50.0f;

	glBegin( GL_LINE_LOOP );
		glVertex3f( v0.x, v0.y, v0.z );
		glVertex3f( v1.x, v1.y, v1.z );
		glVertex3f( v3.x, v3.y, v3.z );
		glVertex3f( v2.x, v2.y, v2.z );
	glEnd();

	glColor3f( 0,0,1 );
	glBegin( GL_LINES );
		glVertex3f( pos.x, pos.y, pos.z );
		glVertex3f( pos.x + normal.x * 10, pos.y + normal.y * 10, pos.z + normal.z * 10 );
	glEnd();
	glPopMatrix();
}

} /* namespace OGLPool */
