/*
 * FpsCamera.h
 *
 *  Created on: 16-03-2013
 *      Author: _CORE7
 */

#ifndef FPSCAMERA_H_
#define FPSCAMERA_H_

#include "Camera.h"

namespace OGLPool {

class FpsCamera: public OGLPool::Camera {
public:
	FpsCamera();
	FpsCamera( vec3 pos );
	virtual ~FpsCamera();

	void setLookAt( vec3 lookAt );
	void rotateY( float angle );
	virtual void update( float dt );
};

} /* namespace OGLPool */
#endif /* FPSCAMERA_H_ */
