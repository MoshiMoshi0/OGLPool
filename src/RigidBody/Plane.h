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
	SetBodyType( PLANE );

	Plane( vec3 normal, vec3 point );
	Plane( vec3 p0, vec3 p1, vec3 p2 );
	virtual ~Plane();
	void render();

	float getBoundingSphereRadius() const;

	vec3 normal;
};

} /* namespace OGLPool */
#endif /* PLANE_H_ */
