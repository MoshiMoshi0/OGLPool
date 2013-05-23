/*
 * BoundingBox.h
 *
 *  Created on: 17-05-2013
 *      Author: _CORE7
 */

#ifndef BOUNDINGBOX_H_
#define BOUNDINGBOX_H_

#include <glm/glm.hpp>
#include <vector>
#include <Shape/Shape.h>
using namespace std;
using namespace glm;

namespace OGLPool {
class BoundingBox : public Shape {
public:
	static bool intersects( const BoundingBox& b0, const BoundingBox& b1 );
	static BoundingBox get( vector< vec3 > points, float skin, bool addSkin = true );
	static BoundingBox get( vec3 min, vec3 max, float skin, bool addSkin = true );

	BoundingBox();
	virtual ~BoundingBox();

	vec3 min;
	vec3 max;
	vec3 minDyn;
	vec3 maxDyn;
	vec3 pos;

	float skin;
};

} /* namespace OGLPool */
#endif /* BOUNDINGBOX_H_ */
