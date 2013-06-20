/*
 * World.h
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <RigidBody/RigidBody.h>
#include <Physics/PhysicsHandler.h>
#include <Util/Table/CueTable.h>

using namespace std;
using namespace OGLPool::Physics;

namespace OGLPool {

class World {
public:
	World();
	virtual ~World();

	void render();
	void update( float dt );

	void addEntity( RigidBody* e );
private:
	vector< RigidBody* > bodies;
	vec3 gravity;

	PhysicsHandler physics;
	CueTable ct;

};

} /* namespace OGLPool */
#endif /* WORLD_H_ */
