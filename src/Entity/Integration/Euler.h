/*
 * Euler.h
 *
 *  Created on: 21-03-2013
 *      Author: _CORE7
 */

#ifndef EULER_H_
#define EULER_H_

#include <Entity/Entity.h>

namespace OGLPool {

class Euler {
public:
	static void integrate( Entity* e, float dt );
};

} /* namespace OGLPool */
#endif /* EULER_H_ */
