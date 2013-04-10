/*
 * World.h
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#ifndef WORLD_H_
#define WORLD_H_

#include <list>
#include <Entity/Entity.h>
#include <Shape/RandomPolygon.h>

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
	list< Entity* > entities;

	RandomPolygon rp;
};

} /* namespace OGLPool */
#endif /* WORLD_H_ */
