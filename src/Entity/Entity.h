/*
 * Entity.h
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#ifndef ENTITY_H_
#define ENTITY_H_

namespace OGLPool {

class Entity {
public:
	Entity();
	virtual ~Entity();

	virtual void render(){};
	virtual void update( float dt ){};
};

} /* namespace OGLPool */
#endif /* ENTITY_H_ */
