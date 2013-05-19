/*
 * Plane.h
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#ifndef PLANE_H_
#define PLANE_H_

#include <RigidBody/RigidBody.h>

namespace OGLPool {

class Plane : public RigidBody {
public:
	Plane( vec3 normal, vec3 point );
	virtual ~Plane();
	void render();

	vec3 normal;
};

} /* namespace OGLPool */
#endif /* PLANE_H_ */
