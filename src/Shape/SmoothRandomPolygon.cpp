/*
 * SmoothRandomPolygon.cpp
 *
 *  Created on: 13-04-2013
 *      Author: DarkNeo
 */

#include "SmoothRandomPolygon.h"
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

namespace OGLPool {

SmoothRandomPolygon::SmoothRandomPolygon():RandomPolygon(10,20) {

	int randEdge = rand() % edges.size();
	int prev = !randEdge ? edges.size() - 1 : randEdge - 1;
	int next = (randEdge + 1) % edges.size();

	vec2 p0, p1, p2, p3, pBezier;
	vector < vec2 > bezierPts;
	vector < Edge2 > bezierEdges;

	p0 = edges[randEdge][0];
	p1 = edges[randEdge][0] - normalize(edges[prev][0] - edges[prev][1]) * 10.f;
	p2 = edges[randEdge][1] - normalize(edges[next][1] - edges[next][0]) * 10.f;
	p3 = edges[randEdge][1];

	//bezier
	int quality = 20;
	float step = 1.0f/quality;
	//for (float t = 0.0f; t < 1; t = glm::clamp( t + step, 0.f, 1.f ) ){
	for( int i = 0; i <= quality; i++ ){
		float t = clamp( i * step, 0.f, 1.f );
		bezierPts.push_back(pow((1.0f - t),3.0f) * p0 + 3 * pow((1.0f - t),2.0f) * t * p1 + 3 * (1.0f - t) * t*t * p2 + t*t*t * p3);
	}

	for (int i = 0; i < bezierPts.size() - 1; i++){
		bezierEdges.push_back(Edge2(bezierPts[i], bezierPts[i + 1]));
	}

	vec2 x = p3 - bezierPts.back();
	cout << x.x << " " << x.y << endl;
	//bezierEdges.push_back( Edge2(bezierPts[ bezierPts.size() - 1] , edges[ randEdge + 1][0]) );
	edges.erase(edges.begin() + randEdge);
	edges.insert(edges.begin() + randEdge, bezierEdges.begin(), bezierEdges.end());
}

SmoothRandomPolygon::~SmoothRandomPolygon() {
}

} /* namespace OGLPool */
