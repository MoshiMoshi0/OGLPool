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

	for( int i = 0; i < 5; ){
		int randEdge = rand() % edges.size();
		if( smoothEdges.count( randEdge ) > 0 ) continue;
		i++;
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

vector< float > SmoothRandomPolygon::tridiagonalSolve(const vector< float >& a, const vector< float >& b, const vector< float >& c, const vector< float >& rhs, uint n){
	vector< float > u(n);
	vector< float > gam(n);

	double bet = b[0];
	u[0] = rhs[0] / bet;

	for (uint j = 1;j < n;j++){
		gam[j] = c[j-1] / bet;
		bet = b[j] - a[j] * gam[j];
		u[j] = (rhs[j] - a[j] * u[j - 1]) / bet;
	}

	for (uint j = 1;j < n;j++)
		u[n - j - 1] -= gam[n - j] * u[n - j];

	return u;
}

vector< float > SmoothRandomPolygon::cyclicSolve( const vector< float >& a, const vector< float >& b, const vector< float >& c, float alpha, float beta, const vector< float >& rhs, uint n ){
	vector< float > bb(n);
	vector< float > x(n);
	vector< float > z(n);

	double gamma = -b[0];
	bb[0] = b[0] - gamma;
	bb[n-1] = b[n - 1] - alpha * beta / gamma;
	for (uint i = 1; i < n - 1; ++i)
	  bb[i] = b[i];

	vector< float > solution = tridiagonalSolve(a, bb, c, rhs, n);
	for (uint k = 0; k < n; ++k)
	  x[k] = solution[k];

	vector< float > u(n);
	u[0] = gamma;
	u[n-1] = alpha;
	for (uint i = 1; i < n - 1; ++i)
	  u[i] = 0.0;

	solution = tridiagonalSolve(a, bb, c, u, n);
	for (uint k = 0; k < n; ++k)
	  z[k] = solution[k];

	double fact = (x[0] + beta * x[n - 1] / gamma) / (1.0 + z[0] + beta * z[n - 1] / gamma);

	for (uint i = 0; i < n; ++i)
	  x[i] -= fact * z[i];

	return x;
}

void SmoothRandomPolygon::createRound(){
	uint n = points.size();
	vector< float > a(n);
	vector< float > b(n);
	vector< float > c(n);

	vector< float > rhsx(n);
	vector< float > rhsy(n);

	for( uint i = 0; i < n; i++ ){
		a[i] = 1; b[i] = 4; c[i] = 1;
	}

	for (uint i = 0; i < n; ++i) {
		uint j = (i == n - 1) ? 0 : i + 1;
		vec2 v = 4.0f * points[i] + 2.0f * points[j];
		rhsx[i] = v.x; rhsy[i] = v.y;
	}

	vector< float > resx = cyclicSolve( a, b, c, 1, 1, rhsx, n );
	vector< float > resy = cyclicSolve( a, b, c, 1, 1, rhsy, n );

	vector< vec2 > first, second;
	for (uint i = 0; i < n; ++i){
		vec2 v( resx[i], resy[i] );
		first.push_back( v );
		second.push_back( 2.0f * points[i] - v );
	}

	for( uint i = 0; i < n; i++ ){
	    int j = (i == n - 1) ? 0 : i + 1;

		vector< vec2 > bezierPts;
		getBezierPoints( points[i], first[i], second[j], points[j], bezierPts, 20 );

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
}

} /* namespace OGLPool */
