/*
 * ContactInfo.h
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#ifndef CONTACTINFO_H_
#define CONTACTINFO_H_

#include <Entity/Entity.h>
#include <glm/glm.hpp>

using namespace glm;

namespace OGLPool {

class ContactInfo {
public:
	enum {
		INVALID = -1, OVERLAPPING = 1, COLLIDING
	};

	ContactInfo( float deltaTime );
	virtual ~ContactInfo();

	bool isOverlapping();
	bool isColliding();
	bool isValid();

	void setOverlapping( bool f );
	void setColliding( bool f );

	Entity* e0;
	Entity* e1;

	vec3 normal;
	vec3 point0;
	vec3 point1;

	float depth;
	float time;

	float deltaTime;
private:
	int contactType;
};

} /* namespace OGLPool */
#endif /* CONTACTINFO_H_ */
