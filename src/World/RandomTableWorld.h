/*
 * RandomTableWorld.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef RANDOMTABLEWORLD_H_
#define RANDOMTABLEWORLD_H_

#include "World.h"
#include <Util/Table/CueTable.h>

namespace OGLPool {

class RandomTableWorld : public World {
public:
	RandomTableWorld();
	virtual ~RandomTableWorld();

	void update( float dt );
};

} /* namespace OGLPool */
#endif /* RANDOMTABLEWORLD_H_ */
