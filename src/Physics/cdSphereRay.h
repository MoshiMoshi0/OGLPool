/*
 * cdSphereRay.h
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#ifndef CDSPHERERAY_H_
#define CDSPHERERAY_H_

#include <Physics/ContactInfo.h>
#include <Entity/Sphere.h>
#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {
namespace Physics {

bool sphereRayTest( vec3 rayOrigin, vec3 rayDir, Sphere* s, ContactInfo* info );
bool sphereRayIntersection( vec3 rayOrigin, vec3 rayDir, vec3 pos, float radius );
bool sphereRayIntersection( vec3 rayOrigin, vec3 rayDir, Sphere* s );
bool sphereRayTest( vec3 rayOrigin, vec3 rayDir, vec3 pos, float radius, ContactInfo* info );

} /* namespace Physics */
} /* namespace OGLPool */
#endif /* CDSPHERERAY_H_ */
