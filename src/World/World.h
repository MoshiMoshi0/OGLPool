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
#include <World/Camera/Camera.h>

using namespace std;

namespace OGLPool {

class World {
public:
	World();
	virtual ~World();

	virtual void render();
	virtual void update( float dt );

	virtual void addBody( RigidBody* e );
protected:
	Camera* camera;
	vector< RigidBody* > bodies;
};

} /* namespace OGLPool */
#endif /* WORLD_H_ */
