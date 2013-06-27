/*
 * GameWorld.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef GameWorld_H_
#define GameWorld_H_

#include <vector>
#include "World.h"
#include <RigidBody/RigidBody.h>
#include <Physics/PhysicsHandler.h>
#include <Util/Table/CueTable.h>

using namespace std;
using namespace OGLPool::Physics;

namespace OGLPool {

class GameWorld : public World {
public:
	GameWorld( Polygon2 shape );
	virtual ~GameWorld();

	void render();
	void update( float dt );
private:
	vec3 gravity;

	PhysicsHandler physics;
	CueTable cueTable;
};

} /* namespace OGLPool */
#endif /* GameWorld_H_ */
