/*
 * SmoothRandomPolygon.cpp
 *
 *  Created on: 13-04-2013
 *      Author: DarkNeo
 */

#include "SmoothRandomPolygon.h"
#include <iostream>
#include <glm/glm.hpp>
#include <SFML/OpenGL.hpp>

using namespace glm;
using namespace std;

namespace OGLPool {

SmoothRandomPolygon::SmoothRandomPolygon() : RandomPolygon(10,15) {
	int randEdge = rand() % edges.size();
	int prev = !randEdge ? edges.size() - 1 : randEdge - 1;
	int next = (randEdge + 1) % edges.size();

	vector<vec2> bezierPts;
	//vector<Edge2> bezierEdges;

	vec2 p0 = edges[randEdge][0];
	vec2 p1 = edges[randEdge][0] - normalize(edges[prev][0] - edges[prev][1]) * 5.f;
	vec2 p2 = edges[randEdge][1] - normalize(edges[next][1] - edges[next][0]) * 5.f;
	vec2 p3 = edges[randEdge][1];
	mat2x4 pMat = transpose( mat4x2( p0, p1, p2, p3 ) );
	mat4 blend( 1.f,-3.f,3.f,-1.f, 0.f,3.f,-6.f,3.f, 0.f,0.f,3.f,-3.f, 0.f,0.f,0.f,1.f );

	uint quality = 40;
	float step = 1.0f/quality;
	for( uint i = 0; i <= quality; i++ ){
		float t = i * step;
		bezierPts.push_back( vec4( 1, t, t*t, t*t*t ) * blend * pMat );
	}

	for (uint i = 0; i < bezierPts.size() - 1; i++){
		bezierEdges.push_back(Edge2(bezierPts[i], bezierPts[i + 1]));
	}

	//edges.erase(edges.begin() + randEdge);
	//edges.insert(edges.begin() + randEdge, bezierEdges.begin(), bezierEdges.end());
}

SmoothRandomPolygon::~SmoothRandomPolygon() {}

void SmoothRandomPolygon::draw(){
	glColor3f( 0,0,1 );
	Polygon::draw();

	glColor3f( 1,0,0 );
	glBegin( GL_LINES );
	for( auto& edge : bezierEdges ){
		glVertex3f( edge[0].x, 0, edge[0].y );
		glVertex3f( edge[1].x, 0, edge[1].y );
	}
	glEnd();
}

} /* namespace OGLPool */
