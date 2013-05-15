/*
 * World.h
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <Entity/Entity.h>
#include <Entity/Plane.h>
#include <Physics/ConstraintSolver/ImpulseConstraintSolver.h>
using namespace std;
namespace OGLPool {

class World {
public:
	World();
	virtual ~World();

	void render();
	void update( float dt );

	void addEntity( Entity* e );
private:
	Physics::ImpulseConstraintSolver solver;
	vector< Entity* > entities;
	vec3 gravity;
};

} /* namespace OGLPool */
#endif /* WORLD_H_ */
