/*
 * Camera.h
 *
 *  Created on: 16-03-2013
 *      Author: _CORE7
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

namespace OGLPool {

class Camera {
public:
	Camera();
	Camera( vec3 pos );
	virtual ~Camera();

	virtual void applyView();
	virtual void setLookAt( vec3 lookAt );
	virtual void update( float dt ){};

	virtual void move( vec3 dir, float speed );
	virtual void moveForward( float speed );
	virtual void moveUp( float speed );
	virtual void moveRight( float speed );

	virtual void rotate( vec3 axis, float angle );
	virtual void rotateX( float angle );
	virtual void rotateY( float angle );
	virtual void rotateZ( float angle );

	vec3 pos;

	vec3 forward;
	vec3 right;
	vec3 up;
};

} /* namespace OGLPool */
#endif /* CAMERA_H_ */
