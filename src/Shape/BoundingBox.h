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

template< class T >
class BoundingBox : public Shape {
public:
	static bool intersects( const BoundingBox<T>& b0, const BoundingBox<T>& b1 );
	static BoundingBox get( vector< T > points, float skin, bool addSkin = true );
	static BoundingBox get( T min, T max, float skin, bool addSkin = true );

	BoundingBox();
	virtual ~BoundingBox();

	void render() const;

	T min;
	T max;
	T minDyn;
	T maxDyn;
	T pos;

	float skin;
};

typedef BoundingBox< vec2 > BoundingBox2;
typedef BoundingBox< vec3 > BoundingBox3;

} /* namespace OGLPool */
#endif /* BOUNDINGBOX_H_ */
