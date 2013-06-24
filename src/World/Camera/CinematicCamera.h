/*
 * CinematicCamera.h
 *
 *  Created on: 24-06-2013
 *      Author: _CORE7
 */

#ifndef CINEMATICCAMERA_H_
#define CINEMATICCAMERA_H_

#include "Camera.h"
#include <glm/glm.hpp>
using namespace glm;

namespace OGLPool {

class CinematicCamera : public Camera {
public:
	CinematicCamera();
	CinematicCamera( vec3 pos );
	virtual ~CinematicCamera();

	void setLookAt( vec3 lookAt );
	void rotateY( float angle );
	void moveUp( float speed );
};

} /* namespace OGLPool */
#endif /* CINEMATICCAMERA_H_ */
