/*
 * SmoothRandomPolygon.h
 *
 *  Created on: 13-04-2013
 *      Author: DarkNeo
 */

#ifndef SMOOTHRANDOMPOLYGON_H_
#define SMOOTHRANDOMPOLYGON_H_

#include "RandomPolygon.h"

namespace OGLPool {

class SmoothRandomPolygon: public OGLPool::RandomPolygon {
public:
	SmoothRandomPolygon();
	virtual ~SmoothRandomPolygon();

	void draw();

	vector<Edge2> bezierEdges;

private:
	void smoothEdge(int randEdge);
};

} /* namespace OGLPool */
#endif /* SMOOTHRANDOMPOLYGON_H_ */
