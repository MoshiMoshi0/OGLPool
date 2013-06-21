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
#include <Util/UDebug/Debug.h>
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

	glVertexPointer(3, GL_FLOAT, 0, value_ptr( vertices[0] ));
	glColorPointer(3, GL_FLOAT, 0, value_ptr( colors[0] ));
	glNormalPointer(GL_FLOAT, 0, value_ptr( normals[0] ));

	glPushMatrix();
	//applyTransform();
	glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	auto debugDraw = Debug::getDebugDraw();
	for( auto& t : triangles ){
		vec3 c = (t[0] + t[1] + t[2]) / 3.0f;
		debugDraw->drawVector( c, t.getNormal() * 2.0f );
	}
}

void Mesh::build(){
	triangles.reserve( triangleCount - 1 );

	for( uint i = 0; i < triangleCount; i++ ){
		uint i0 = i * 3 + 0;
		uint i1 = i * 3 + 1;
		uint i2 = i * 3 + 2;

		vec3 v0 = vertices[i0];
		vec3 v1 = vertices[i1];
		vec3 v2 = vertices[i2];

		triangles.push_back( Triangle3( v0, v1, v2 ) );
	}

	boundingBox = BoundingBox3::get( vertices, 0.1f );
	pos = boundingBox.pos;
}

void Mesh::calculateNormals(){
	assert( vertices.size() % 3 == 0 );

	uint i0 = triangleCount * 3 + 0;
	uint i1 = triangleCount * 3 + 1;
	uint i2 = triangleCount * 3 + 2;

	vec3 a = vertices[i0];
	vec3 b = vertices[i1];
	vec3 c = vertices[i2];

	vec3 n = normalize( cross( b-a, c-a ) );

	normals.push_back( n );
	normals.push_back( n );
	normals.push_back( n );
	//normals[i0] = n;
	//normals[i1] = n;
	//normals[i2] = n;
}

void Mesh::vertex( vec3 v ){ vertices.push_back( v ); }
void Mesh::normal( vec3 n ){ normals.push_back( n ); }
void Mesh::color( vec3 c ){ colors.push_back( c ); }

float Mesh::getBoundingSphereRadius() const { return glm::max( length( boundingBox.min ), length( boundingBox.max ) ); }

} /* namespace OGLPool */
