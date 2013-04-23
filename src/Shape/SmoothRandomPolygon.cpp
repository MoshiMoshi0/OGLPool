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
#include <unordered_set>

using namespace glm;
using namespace std;

namespace OGLPool {

SmoothRandomPolygon::SmoothRandomPolygon( Type type ) : RandomPolygon(10,15) {
	switch ( type ) {
		case RANDOM: createRandom(); break;
		case ROUND: createRound(); break;
		default: assert(0);
	}
}

SmoothRandomPolygon::~SmoothRandomPolygon() {}

void SmoothRandomPolygon::createRandom(){
	unordered_set<int> smoothEdges;

	while( smoothEdges.size() < 5 ){
		int randEdge = rand() % edges.size();
		if( smoothEdges.count( randEdge ) > 0 ) continue;
		smoothEdges.emplace( randEdge );
		smoothEdge(randEdge);
	}
}

void SmoothRandomPolygon::smoothEdge(int randEdge){
	int prev = !randEdge ? edges.size() - 1 : randEdge - 1;
	int next = (randEdge + 1) % edges.size();

	vec2 p0 = edges[randEdge][0];
	vec2 p1 = edges[randEdge][0] - normalize(edges[prev][0] - edges[prev][1]) * 5.f;
	vec2 p2 = edges[randEdge][1] - normalize(edges[next][1] - edges[next][0]) * 5.f;
	vec2 p3 = edges[randEdge][1];

	vector<vec2> bezierPts;
	getBezierPoints( p0, p1, p2, p3, bezierPts, 20 );

	for (uint i = 0; i < bezierPts.size() - 1; i++) {
		bezierEdges.push_back(Edge2(bezierPts[i], bezierPts[i + 1]));
	}

	//edges.erase(edges.begin() + randEdge);
	//edges.insert(edges.begin() + randEdge, bezierEdges.begin(), bezierEdges.end());
}

vector<float> SmoothRandomPolygon::tridiagonalSolve(const vector<float>& a, const vector<float>& b, const vector<float>& c, const vector<float>& rhs, uint n){
	vector<float> u(n);
	vector<float> gam(n);

	double bet = b[0];
	u[0] = rhs[0] / bet;

	for (uint i = 1; i < n; i++){
		gam[i] = c[i-1] / bet;
		bet = b[i] - a[i] * gam[i];
		u[i] = (rhs[i] - a[i] * u[i - 1]) / bet;
	}

	for (uint i = 1; i < n; i++)
		u[n - i - 1] -= gam[n - i] * u[n - i];

	return u;
}

vector<float> SmoothRandomPolygon::cyclicSolve( const vector<float>& a, const vector<float>& b, const vector<float>& c, float alpha, float beta, const vector<float>& rhs, uint n ){
	vector<float> bb(n);
	vector<float> x(n);
	vector<float> z(n);

	float gamma = -b[0];
	bb[0] = b[0] - gamma;
	bb[n-1] = b[n - 1] - alpha * beta / gamma;
	for (uint i = 1; i < n - 1; i++)
	  bb[i] = b[i];

	vector<float> solution = tridiagonalSolve(a, bb, c, rhs, n);
	for (uint i = 0; i < n; i++)
	  x[i] = solution[i];

	vector<float> u(n);
	u[0] = gamma;
	u[n-1] = alpha;
	for (uint i = 1; i < n - 1; i++)
	  u[i] = 0.0;

	solution = tridiagonalSolve(a, bb, c, u, n);
	for (uint i = 0; i < n; i++)
	  z[i] = solution[i];

	float fact = (x[0] + beta * x[n - 1] / gamma) / (1.0 + z[0] + beta * z[n - 1] / gamma);

	for (uint i = 0; i < n; i++)
	  x[i] -= fact * z[i];

	return x;
}

void SmoothRandomPolygon::createRound(){
	float k = 0.5f;
	uint n = points.size();
	vector<float> a(n);
	vector<float> b(n);
	vector<float> c(n);

	vector<float> rhsx(n);
	vector<float> rhsy(n);

	for( uint i = 0, j = n - 1; i < n; j = i++ ){
		a[i] = 1; b[i] = 4; c[i] = 1;
		rhsx[j] = 4.0f * points[j].x + 2.0f * points[i].x;
		rhsy[j] = 4.0f * points[j].y + 2.0f * points[i].y;
	}

	vector<float> resx = cyclicSolve( a, b, c, 1, 1, rhsx, n );
	vector<float> resy = cyclicSolve( a, b, c, 1, 1, rhsy, n );

	vector<vec2> first, second;
	for( uint i = 0; i < n; i++ ){
		vec2 resv = vec2( resx[i], resy[i] );
		first.push_back( points[i] * (1 - k) + resv * k );
		second.push_back( points[i] * (1 + k) - resv * k );
	}

	for( uint i = 0, j = n - 1; i < n; j = i++ ){
		vector< vec2 > bezierPts;
		getBezierPoints( points[j], first[j], second[i], points[i], bezierPts, 20 );

		bezierControlEdges.push_back( Edge2(points[j], first[j]) );
		bezierControlEdges.push_back( Edge2(second[i], points[i]) );
		for (uint k = 0; k < bezierPts.size() - 1; k++) {
			bezierEdges.push_back(Edge2(bezierPts[k], bezierPts[k + 1]));
		}
	}
}

void SmoothRandomPolygon::getBezierPoints( const vec2& p0, const vec2& p1, const vec2& p2, const vec2& p3, vector<vec2>& bezierPts, uint quality ){
	static const mat4 blend(1.f, -3.f, 3.f, -1.f, 0.f, 3.f, -6.f, 3.f, 0.f, 0.f, 3.f, -3.f, 0.f, 0.f, 0.f, 1.f);

	float step = 1.0f / quality;

	mat2x4 pMat = transpose(mat4x2(p0, p1, p2, p3));
	for (uint i = 0; i <= quality; i++) {
		float t = i * step;
		bezierPts.push_back(vec4(1, t, t * t, t * t * t) * blend * pMat);
	}
}

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

	glColor3f( 0,1,0 );
	glBegin( GL_LINES );
	for( auto& edge : bezierControlEdges ){
		glVertex3f( edge[0].x, 0, edge[0].y );
		glVertex3f( edge[1].x, 0, edge[1].y );
	}
	glEnd();
}

} /* namespace OGLPool */
