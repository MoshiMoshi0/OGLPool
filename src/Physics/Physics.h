/*
 * Physics.h
 *
 *  Created on: 19-03-2013
 *      Author: _UX31A
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <Physics/ContactInfo.h>
#include <Entity\Sphere.h>
#include <Physics\CollisionDetection.h>
#include <Physics/ImpulseCollisionResponse.h>

namespace OGLPool {
	namespace Physics {
		bool checkCollision( Entity* e0, Entity* e1, ContactInfo* info ){
			auto *i0 = &typeid (*e0);
			auto *i1 = &typeid (*e1);

			if (*i0 == typeid(Sphere)) {
				if (*i1 == typeid(Sphere)) return sphereSphereTest((Sphere*) e0, (Sphere*) e1, info);
				if (*i1 == typeid(Plane)) return spherePlaneTest((Sphere*) e0, (Plane*) e1, info);
			}
			if (*i0 == typeid(Plane)) {
				if (*i1 == typeid(Sphere)) return spherePlaneTest((Sphere*) e1, (Plane*) e0, info );
			}

			return false;
		}
	} /* namespace Physics */
} /* namespace OGLPool */
#endif /* PHYSICS_H_ */
