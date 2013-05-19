/*
 * cdSpherePlane.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef CDSPHEREPLANE_H_
#define CDSPHEREPLANE_H_

#include <Physics/ManifoldPoint.h>
#include <RigidBody/Plane.h>
#include <RigidBody/Sphere.h>
#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {
namespace Physics {

bool spherePlaneIntersection();
bool spherePlaneSwept( Sphere* s, Plane* p, ManifoldPoint* info );
bool spherePlaneOverlap( Sphere* s, Plane* p, ManifoldPoint* info );
bool spherePlaneTest( Sphere* s, Plane* p, ManifoldPoint* info );

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CDSPHEREPLANE_H_ */
