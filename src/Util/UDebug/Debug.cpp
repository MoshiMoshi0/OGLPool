/*
 * Debug.cpp
 *
 *  Created on: 19-05-2013
 *      Author: _CORE7
 */

#include "Debug.h"
#include <assert.h>

namespace OGLPool {

DebugDraw* Debug::debugDraw = 0;
void Debug::init(){
	debugDraw = new DebugDraw();
}

void Debug::deinit(){
	delete debugDraw;
}

void Debug::render(){
	debugDraw->render();
}

DebugDraw* Debug::getDebugDraw(){
	assert( debugDraw );
	return debugDraw;
}

} /* namespace OGLPool */
