/*
 * Sphere.h
 *
 *  Created on: 18-03-2013
 *      Author: _CORE7
 */

#ifndef SPHERE_H_
#define SPHERE_H_

#include <Entity/Entity.h>
#include <SFML/OpenGL.hpp>

namespace OGLPool {

class Sphere : public Entity {
public:
	Sphere( float radius );
	Sphere( float radius, vec3 pos );
	virtual ~Sphere();

	void update( float dt );
	void render();

	float radius;
private:
	GLUquadric* quadric;
	int gluSlices;
	int gluStacks;
};

} /* namespace OGLPool */
#endif /* SPHERE_H_ */