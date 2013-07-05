/*
 * Bezier.h
 *
 *  Created on: 05-07-2013
 *      Author: _UX31A
 */

#ifndef BEZIER_H_
#define BEZIER_H_

#include "Shape.h"
#include <glm/glm.hpp>
#include <Shape/Edge.h>
#include <vector>
using namespace glm;
using namespace std;

namespace OGLPool {

template< class T >
class Bezier : public Shape {
public:
	Bezier();
	Bezier( vector<T> controlPoints, uint quality  );
	virtual ~Bezier();

	void create( const vector<T>& controlPoints, uint quality );

	vector<T> controlPoints;
	vector<T> points;
	vector< Edge<T> > edges;
};

typedef Bezier< vec2 > Bezier2;
typedef Bezier< vec3 > Bezier3;

} /* namespace OGLPool */
#endif /* BEZIER_H_ */
