/*
 * Delaunay.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "Delaunay.h"
#include <Shape/Circle.h>
#include <algorithm>
using namespace std;

namespace OGLPool {

Delaunay::Delaunay() { state = DState::VALID; }

Delaunay::~Delaunay() {}

bool Delaunay::isValid(){
	return state == DState::VALID;
}

vector< ivec3 > Delaunay::getTriangleIndices(){
	vector< ivec3 > ret;

	for( int i = 0; i < nFaces; i++ ){
		bool firstEdge = true;
		int i0, i1, i2;

		for( int j = 0; j < edges.size(); j++ ){
			DEdge e = edges[j];

			if( i == e.l || i == e.r ){
				if( firstEdge ){
					i0 = e.s;
					i1 = e.t;
					firstEdge = false;
				}else{
					i2 = (e.s == i0 || e.s == i1) ? e.t : e.s;
					break;
				}
			}
		}

		ret.push_back( ivec3(i0,i1,i2) );
	}

	return ret;
}

void Delaunay::triangulate( const vector< vec2 >& points ){
	if( state == DState::INVALID ) return;
	this->points = vector<vec2> (points);

	u = s = t = bP = nFaces = 0;
	int s, t;
	findClosestNeighbours( s, t );

	addEdge(s, t, UNDEFINED, UNDEFINED);

	unsigned int currentEdge = 0;
	while (currentEdge < edges.size()) {
	    if (edges[currentEdge].l == UNDEFINED)
			completeFacet(currentEdge);
	    if (edges[currentEdge].r == UNDEFINED)
			completeFacet(currentEdge);
	    currentEdge++;
	}

	state = DState::INVALID;
}

void Delaunay::triangulate( const vector< vec2 >& points, vector< Triangle2 >& tris ){
	if( state == DState::INVALID ) return;
	triangulate( points );
	vector< Triangle2 > ret = getTriangles();
	tris.insert( tris.begin(), ret.begin(), ret.end() );
}

void Delaunay::triangulate( const vector< vec3 >& points3, vector< Triangle3 >& tris ){
	if( state == DState::INVALID ) return;
	if( !tris.empty() ) tris.clear();
	vector< vec2 > points2;
	for( unsigned int i = 0; i < points3.size(); i++ ){
		points2.push_back( vec2( points3[i].x, points3[i].z ) );
	}

	triangulate( points2 );

	vector< ivec3 > idx = getTriangleIndices();
	for( unsigned int i = 0; i < idx.size(); i++ ){
		tris.push_back( Triangle3( points3[ idx[i].x ], points3[ idx[i].y ], points3[ idx[i].z ] ) );
	}
}

vector< DEdge > Delaunay::getDEdges(){
	return edges;
}

vector< Edge2 > Delaunay::getEdges(){
	vector< Edge2 > es;

	for( unsigned int i = 0; i < edges.size(); i++ ){
		DEdge de = edges.at( i );
		es.push_back( Edge2( points[ de.s ], points[ de.t ] ) );
	}

	return es;
}

vector< Triangle2 > Delaunay::getTriangles(){
	vector< ivec3 > idx = getTriangleIndices();
	vector< Triangle2 > ts;

	for( unsigned int i = 0; i < idx.size(); i++ ){
		ivec3 v = idx.at( i );
		ts.push_back( Triangle2( points[ v.x ], points[ v.y ], points[ v.z ] ) );
	}

	return ts;
}

vector< vec2 > Delaunay::getPoints(){
	return points;
}

void Delaunay::completeFacet(int eI) {
	if (edges[eI].l == UNDEFINED) {
		s = edges[eI].s;
		t = edges[eI].t;
	} else if (edges[eI].r == UNDEFINED) {
		s = edges[eI].t;
		t = edges[eI].s;
	} else {
		return;
	}

	for (u = 0; u < points.size(); u++){
		if ( u == s || u == t )
			continue;

		if ( determinant( mat2x2( points[s] - points[u], points[t] - points[u] ) ) > 0.0 )
			break;
	}

	bP = u;
	if ( bP < points.size() ){
		Circle bC = Circle::circumCircle( points[s], points[t], points[bP] );

		for ( u = bP + 1; u < points.size(); u++ ) {
			if ( u == s || u == t )
				continue;

			float cP = determinant( mat2x2( points[s] - points[u], points[t] - points[u] ) );
			if (cP > 0.0f){
				if (bC.inside( points[u] )){
					bP = u;
					bC = Circle::circumCircle(points[s], points[t], points[u]);
				}
			}
		}
	}

	if ( bP < points.size() ){
		updateLeftFace(eI, s, t, nFaces);

		eI = findEdge(bP, s);
		if (eI == UNDEFINED){
			eI = addEdge(bP, s, nFaces, UNDEFINED);
		}else{
			updateLeftFace(eI, bP, s, nFaces);
		}

		eI = findEdge(t, bP);
		if (eI == UNDEFINED){
			eI = addEdge(t, bP, nFaces, UNDEFINED);
		} else {
			updateLeftFace(eI, t, bP, nFaces);
		}
		nFaces++;
	} else {
		updateLeftFace(eI, s, t, UNIVERSE);
	}
}

void Delaunay::findClosestNeighbours( int& u, int& v) {
	float d, min;

	min = std::numeric_limits<float>::max();
	for (unsigned int i = 0; i < points.size()-1; i++) {
	    for (unsigned int j = i+1; j < points.size(); j++) {
	    	d = distance( points[i], points[j] );
		    if (d < min) {
			    u = i;
			    v = j;
			    min = d;
			}
		}
	}
}

int Delaunay::addEdge( int s, int t ){
	return addEdge(s, t, UNDEFINED, UNDEFINED);
}

int Delaunay::addEdge( int s, int t, int l, int r ){
	int e = findEdge(s, t);

	if (e == UNDEFINED) {
		if (s < t) {
			edges.push_back( DEdge(s,t,l,r) );
		} else {
			edges.push_back( DEdge(t,s,r,l) );
		}
		return edges.size() - 1;
	} else {
		return UNDEFINED;
	}
}

int Delaunay::findEdge(int s, int t){
	for (unsigned int i = 0; i < edges.size(); i++){
	    if ( (edges[i].s == s && edges[i].t == t) || (edges[i].s == t && edges[i].t == s)) {
	    	return i;
	    }
	}

	return DEdgeType::UNDEFINED;
}

void Delaunay::updateLeftFace(int eI, int s, int t, int f){
	if (!((edges[eI].s == s && edges[eI].t == t) || (edges[eI].s == t && edges[eI].t == s))){
		//error( "edge table mismatch" ); @TODO ?
	}

	if (edges[eI].s == s && edges[eI].l == UNDEFINED){
		edges[eI].l = f;
	}else if (edges[eI].t == s && edges[eI].r == UNDEFINED){
		edges[eI].r = f;
	}else{
		//error( "attempt to overwrite edge info" ); @TODO ?
	}
}

} /* namespace OGLPool */
