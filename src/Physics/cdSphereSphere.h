/*
 * cdSphereSphere.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef CDSPHERESPHERE_H_
#define CDSPHERESPHERE_H_

#include <Physics/ContactInfo.h>
#include <Entity/Sphere.h>
#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {
namespace Physics {

bool sphereSphereIntersection( Sphere* s0, Sphere* s1 );
bool sphereSphereSwept( Sphere* s0, Sphere* s1, ContactInfo* info );
bool sphereSphereOverlap( Sphere* s0, Sphere* s1, ContactInfo* info );
bool sphereSphereTest( Sphere* s0, Sphere* s1, ContactInfo* info );

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CDSPHERESPHERE_H_ */
