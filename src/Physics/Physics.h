/*
 * Physics.h
 *
 *  Created on: 19-03-2013
 *      Author: _UX31A
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <Entity\Sphere.h>
namespace OGLPool {
	namespace Physics {

		bool sphereSphereIntersection( Sphere* s0, Sphere* s1 ){
			return length(s0->pos + s1->pos) <= s0->radius + s1->radius;
		}

	} /* namespace Physics */
} /* namespace OGLPool */
#endif /* PHYSICS_H_ */
