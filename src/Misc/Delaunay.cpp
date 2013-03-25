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

Delaunay::Delaunay() { state = VALID; }

Delaunay::~Delaunay() {}

vector< ivec3 > Delaunay::getTriangleIndices(){
	vector< ivec3 > ret;
	for (unsigned int i = 0; i < edges.size(); ++i) {
		DEdge e0 = edges[i];
		for (unsigned int j = i + 1; j < edges.size(); ++j) {
			DEdge e1 = edges[j];
			if( e0.t == e1.s || e0.t == e1.t ){

				int f0 = 0;
				if( e0.t == e1.t ){
					f0 = findEdge( e0.s, e1.s );
				} else {
					f0 = findEdge(e0.s, e1.t );
				}

				if( f0 != UNDEFINED ){
					DEdge e2 = edges[ f0 ];

					int i0 = e2.s;
					int i1 = e2.t;
					int i2 = e0.t;

					Circle bC = Circle::circumCircle( points[i0], points[i1], points[i2] );
					bool empty = true;
					for( unsigned int l = 0; l < points.size(); l++ ){
						if( l == i0 || l == i1 || l == i2 )
							continue;

						if( bC.inside( points[l] )){
							empty = false;
							break;
						}
					}

					if( empty ){
						ivec3 x = ivec3( i0, i1, i2 );
						if( find(ret.begin(), ret.end(), x) != ret.end())
							continue;

						ret.push_back( x );
					}
				}
			}
		}
	}
	return ret;
}

void Delaunay::triangulate( vector< vec2 > points ){
	if( state == INVALID ) return;
	this->points = vector<vec2> (points);

	int nFaces = 0;
	int s, t;

	findClosestNeighbours( s, t );

	addEdge(s, t, UNDEFINED, UNDEFINED);

	unsigned int currentEdge = 0;
	while (currentEdge < edges.size()) {
	    if (edges[currentEdge].l == UNDEFINED)
			completeFacet(currentEdge, nFaces);
	    if (edges[currentEdge].r == UNDEFINED)
			completeFacet(currentEdge, nFaces);
	    currentEdge++;
	}

	state = INVALID;
}

void Delaunay::triangulate( vector< vec2 > points, vector< Triangle2 >& tris ){
	if( state == INVALID ) return;
	triangulate( points );
	vector< Triangle2 > ret = getTriangles();
	tris.insert( tris.begin(), ret.begin(), ret.end() );
}

void Delaunay::triangulate( vector< vec3 > points3, vector< Triangle3 >& tris ){
	if( state == INVALID ) return;
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

void Delaunay::completeFacet(int eI, int nFaces) {
	unsigned int u, s, t;

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

	unsigned int bP = u;
	if ( bP < points.size() ){
		Circle bC = Circle::circumCircle( points[s], points[t], points[bP] );

		for ( u = bP + 1; u < points.size(); u++ ) {
			if ( u == s || u == t )
				continue;

			float cP = determinant( mat2x2( points[s] - points[u], points[t] - points[u] ) );
			if (cP > 0.0){
				if (bC.inside( points[u] )){
					bP = u;
					bC = Circle::circumCircle(points[s], points[t], points[u]);
				}
			}
		}
	}

	if ( bP < points.size() ){
		updateLeftFace(eI, s, t, nFaces);
		nFaces++;

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
	} else {
		updateLeftFace(eI, s, t, UNIVERSE);
	}
}

void Delaunay::findClosestNeighbours( int& u, int& v) {
	unsigned int i, j;
	float d, min;
	int s, t;

	s = t = 0;
	min = std::numeric_limits<float>::max();
	for (i = 0; i < points.size()-1; i++) {
	    for (j = i+1; j < points.size(); j++) {
	    	d = distance( points[i], points[j] );
		    if (d < min) {
			    s = i;
			    t = j;
			    min = d;
			}
		}
	}
	u = s;
	v = t;
}

int Delaunay::addEdge( int s, int t ){
	return addEdge(s, t, UNDEFINED, UNDEFINED);
}

int Delaunay::addEdge( int s, int t, int l, int r ){
	int e;

	e = findEdge(s, t);
	if (e == UNDEFINED) {
		if (s < t) {
			edges.push_back( DEdge( s,t,l,r ) );
		} else {
			edges.push_back( DEdge(t,s,r,l) );
		}
		return edges.size() - 1;
	} else {
		return UNDEFINED;
	}
}

int Delaunay::findEdge(int s, int t){
	bool edgeExists = false;

	unsigned int i;
	for (i = 0; i < edges.size(); i++){
	    if ( (edges[i].s == s && edges[i].t == t) || (edges[i].s == t && edges[i].t == s)) {
			edgeExists = true;
			break;
	    }
	}

	if (edgeExists) return i;
	else return UNDEFINED;
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
