/*
 * Shape.h
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#ifndef SHAPE_H_
#define SHAPE_H_

namespace OGLPool {

class Shape {
public:
	Shape();
	virtual ~Shape();

	virtual void render(){}
};

} /* namespace OGLPool */
#endif /* SHAPE_H_ */
