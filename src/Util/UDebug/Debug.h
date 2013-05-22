/*
 * Debug.h
 *
 *  Created on: 19-05-2013
 *      Author: _CORE7
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "DebugDraw.h"

namespace OGLPool {

class Debug {
public:
	static void init();
	static void deinit();
	static void render();

	static DebugDraw* getDebugDraw();
private:
	static DebugDraw* debugDraw;
};

} /* namespace OGLPool */
#endif /* DEBUG_H_ */
