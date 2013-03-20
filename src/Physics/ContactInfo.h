/*
 * ContactInfo.h
 *
 *  Created on: 19-03-2013
 *      Author: _CORE7
 */

#ifndef CONTACTINFO_H_
#define CONTACTINFO_H_

#include <glm/glm.hpp>

using namespace glm;

namespace OGLPool {

class ContactInfo {
public:
	enum {
		INVALID = -1, OVERLAPPING = 1, COLLIDING
	};

	ContactInfo();
	virtual ~ContactInfo();

	vec3 getNormal();
	vec3 getContactPoint();
	float getDepth();
	float getContactTime();
	bool isOverlapping();
	bool isColliding();
	bool isValid();

	void setNormal(vec3 normal);
	void setContactPoint(vec3 point);
	void setDepth(float depth);
	void setContactTime( float time );
	void setOverlapping( bool f );
	void setColliding( bool f );
private:
	vec3 normal;
	vec3 point;

	float depth;
	float time;

	int contactType;
};

} /* namespace OGLPool */
#endif /* CONTACTINFO_H_ */
