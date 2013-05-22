/*
 * DebugDraw.h
 *
 *  Created on: 19-05-2013
 *      Author: _CORE7
 */

#ifndef DEBUGDRAW_H_
#define DEBUGDRAW_H_

#include <glm/glm.hpp>
#include <queue>
#include <tuple>
#include <stack>
using namespace glm;
using namespace std;

namespace OGLPool {

class DebugDraw {
public:
	DebugDraw();
	virtual ~DebugDraw();

	DebugDraw* drawLine( vec3 p0, vec3 p1 );
	DebugDraw* drawVector( vec3 p, vec3 v );
	DebugDraw* drawPlane( vec3 p, vec3 n, float size );

	DebugDraw* setColor( float r, float g, float b );
	void render();
private:
	vec3 color;
	queue< tuple< vec3, vec3, vec3 > > lines;
};

} /* namespace OGLPool */
#endif /* DEBUGDRAW_H_ */
