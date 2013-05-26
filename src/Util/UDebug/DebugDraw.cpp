/*
 * DebugDraw.cpp
 *
 *  Created on: 19-05-2013
 *      Author: _CORE7
 */

#include "DebugDraw.h"
#include <Util/MathUtil.h>
#include <SFML/OpenGL.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OGLPool {

DebugDraw::DebugDraw(){ setColor( 1,1,1 ); };
DebugDraw::~DebugDraw(){};

DebugDraw* DebugDraw::drawLine( vec3 p0, vec3 p1 ){
	lines.push( std::tuple< vec3,vec3,vec3 >(p0, p1, color) );
	return this;
}
DebugDraw* DebugDraw::drawVector( vec3 p, vec3 v ){ return drawLine( p, p + v ); }

DebugDraw* DebugDraw::drawPlane( vec3 p, vec3 n, float size ){
	vec3 t0,t1;
	planeSpace( n, t0, t1 );

	vec3 v0 = p + t0 * size + t1 * size;
	vec3 v1 = p + t0 * size - t1 * size;
	vec3 v2 = p - t0 * size + t1 * size;
	vec3 v3 = p - t0 * size - t1 * size;

	return drawLine( v0, v1 )->drawLine( v1, v3 )->drawLine( v3, v2 )->drawLine( v2, v0 );
}

DebugDraw* DebugDraw::setColor( float r, float g, float b ){
	color.r = r; color.g = g; color.b = b;
	return this;
}

void DebugDraw::render(){
	glDisable(GL_LIGHTING);
	for(; !lines.empty(); lines.pop() ){
		auto& t = lines.front();
		vec3 p0 = get<0>(t);
		vec3 p1 = get<1>(t);
		vec3 lc = get<2>(t);

		glColor3f( lc.r, lc.g, lc.b );
		glBegin( GL_LINES );
			glVertex3f( p0.x, p0.y, p0.z );
			glVertex3f( p1.x, p1.y, p1.z );
		glEnd();
	}
	glEnable(GL_LIGHTING);
	setColor( 1,1,1 );
}

} /* namespace OGLPool */
