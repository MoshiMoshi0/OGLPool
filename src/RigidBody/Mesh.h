/*
 * Mesh.h
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include <glm/glm.hpp>
#include <RigidBody/RigidBody.h>
#include <Shape/Triangle.h>
using namespace glm;
using namespace std;

namespace OGLPool {

class Mesh : public RigidBody {
public:
	SetBodyType( MESH );

	Mesh();
	virtual ~Mesh();

	void render();
	void build();

	void beginTriangle();
	void endTriangle();

	void calculateNormals();

	void vertex( vec3 v );
	void normal( vec3 n );
	void color( vec3 c );

	vector< vec3 > vertices;
	vector< vec3 > normals;
	vector< vec3 > colors;

	vector< Triangle3 > triangles;

	uint triangleCount;
};

} /* namespace OGLPool */
#endif /* MESH_H_ */
