/*
 * Mesh.cpp
 *
 *  Created on: 22-05-2013
 *      Author: _CORE7
 */

#include "Mesh.h"
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
using namespace std;

namespace OGLPool {

Mesh::Mesh() : RigidBody() {
	setInertia( mat3(0.0f) );

	restitutionCoef = 0.6f;
	frictionCoef = 0.5f;
	rollingFrictionCoef = 0.5f;

	triangleCount = 0;
}

Mesh::~Mesh(){}

void Mesh::beginTriangle(){
	assert( vertices.size() % 3 == 0 );
	assert( normals.size() % 3 == 0 );
	assert( colors.size() % 3 == 0 );

	vertices.reserve( vertices.size() + 3 );
	normals.reserve( normals.size() + 3 );
	colors.reserve( colors.size() + 3 );
}

void Mesh::endTriangle(){
	assert( vertices.size() % 3 == 0 );
	assert( normals.size() % 3 == 0 );
	assert( colors.size() % 3 == 0 );
	triangleCount++;
}

void Mesh::render(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	glVertexPointer(triangleCount * 3, GL_FLOAT, 0, value_ptr( vertices[0] ));
	glColorPointer(triangleCount * 3, GL_FLOAT, 0, value_ptr( colors[0] ));
	glNormalPointer(GL_FLOAT, 0, value_ptr( normals[0] ));

	glPushMatrix();
	applyTransform();
	glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

void Mesh::build(){
	triangles.reserve( triangleCount );

	for( int i = 0; i < triangleCount; i++ ){
		int i0 = i * 3 + 0;
		int i1 = i * 3 + 1;
		int i2 = i * 3 + 2;

		vec3 v0 = vertices[i0];
		vec3 v1 = vertices[i1];
		vec3 v2 = vertices[i2];

		triangles[i] = Triangle3( v0, v1, v2 );
	}

	boundingBox = BoundingBox::get( vertices, 0.1f );
}

vector< Triangle3 >* Mesh::getTriangles(){
	return &triangles;
}

void Mesh::calculateNormals(){
	assert( vertices.size() % 3 == 0 );
	vec3 a = vertices[ triangleCount * 3 + 0 ];
	vec3 b = vertices[ triangleCount * 3 + 1 ];
	vec3 c = vertices[ triangleCount * 3 + 2 ];

	vec3 n = normalize( cross( b-a, c-a ) );

	normals[ triangleCount * 3 + 0 ] = n;
	normals[ triangleCount * 3 + 1 ] = n;
	normals[ triangleCount * 3 + 2 ] = n;
}

void Mesh::vertex( vec3 v ){ vertices.push_back( v ); }
void Mesh::normal( vec3 n ){ normals.push_back( n ); }
void Mesh::color( vec3 c ){ colors.push_back( c ); }

} /* namespace OGLPool */
