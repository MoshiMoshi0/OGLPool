/*
 * Delaunay.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "Delaunay.h"
#include <Shape/Circle.h>
#include <iostream>
#include <algorithm>
using namespace std;

namespace OGLPool {

Delaunay::Delaunay() {}

Delaunay::~Delaunay() {}

vector< ivec3 > Delaunay::constructTris(){
	vector< ivec3 > ret;
	for (unsigned int i = 0; i < edge.size(); ++i) {
		DEdge e0 = edge[i];
		for (unsigned int j = i + 1; j < edge.size(); ++j) {
			DEdge e1 = edge[j];
			if( e0.t == e1.s || e0.t == e1.t ){

				int f0 = 0;
				if( e0.t == e1.t ){
					f0 = findEdge( e0.s, e1.s );
				} else {
					f0 = findEdge(e0.s, e1.t );
				}

				if( f0 != UNDEFINED ){
					DEdge e2 = edge[ f0 ];

					int i0 = e2.s;
					int i1 = e2.t;
					int i2 = e0.t;
					Circle bC = Circle::circumCircle( point[i0], point[i1], point[i2] );
					bool empty = true;

					for( int l = 0; l < point.size(); l++ ){
						if( l == i0 || l == i1 || l == i2 )
							continue;
						if( bC.inside( point[l] )){
							empty = false;
							break;
						}
					}

					if( empty ){
						ivec3 x = ivec3( i0, i1, i2 );
						if( find(ret.begin(), ret.end(), x) != ret.end())
							continue;

						ret.push_back( x );
						cout << i0 << " " << i1 << " " << i2 << endl;
					}
				}
			}
		}
	}
	return ret;
}

void Delaunay::triangulate( vector< vec2 > points ){
	point = vector<vec2> (points);

	unsigned int currentEdge;
	int nFaces = 0;
	int s, t;

	findClosestNeighbours( s, t );

	addEdge(s, t, UNDEFINED, UNDEFINED);

	currentEdge = 0;
	while (currentEdge < edge.size()) {
	    if (edge[currentEdge].l == UNDEFINED)
			completeFacet(currentEdge, nFaces);
	    if (edge[currentEdge].r == UNDEFINED)
			completeFacet(currentEdge, nFaces);
	    currentEdge++;
	}
}

float triNorm( vec2 p1, vec2 p2, vec2 p3 ){
	vec2 v21 = p2 - p1;
	vec2 v31 = p3 - p1;
	return v21.x * v31.y - v21.y * v31.x;
}

void Delaunay::completeFacet(int eI, int nFaces) {
	float cP;
	unsigned int bP, u, s, t;

	if (edge[eI].l == UNDEFINED) {
		s = edge[eI].s;
		t = edge[eI].t;
	} else if (edge[eI].r == UNDEFINED) {
		s = edge[eI].t;
		t = edge[eI].s;
	}else return;

	for (u = 0; u < point.size(); u++){
		if (u == s || u == t)
			continue;
		if (triNorm(point[s], point[t], point[u]) > 0.0)
			break;
	}

	bP = u;
	if ( bP < point.size() ){
		Circle bC = Circle::circumCircle( point[s], point[t], point[bP] );

		for (u = bP+1; u < point.size(); u++) {
			if (u == s || u == t)
				continue;

			cP = triNorm(point[s], point[t], point[u]);

			if (cP > 0.0){
				if (bC.inside(point[u])){
					bP = u;
					bC = Circle::circumCircle(point[s], point[t], point[u]);
				}
			}
		}
	}

	if ( bP < point.size() ){
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
	min = 1e30f; //@TODO
	for (i = 0; i < point.size()-1; i++) {
	    for (j = i+1; j < point.size(); j++) {
	    	d = distance( point[i], point[j] );
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

/*void Delaunay::addTriangle( int s, int t, int u ){
	addEdge(s, t);
	addEdge(t, u);
	addEdge(u, s);
}*/

int Delaunay::addEdge( int s, int t ){
	return addEdge(s, t, UNDEFINED, UNDEFINED);
}

int Delaunay::addEdge( int s, int t, int l, int r ){
	int e;

	e = findEdge(s, t);
	if (e == UNDEFINED) {
		if (s < t) {
			DEdge e;
			e.s = s;
			e.t = t;
			e.l = l;
			e.r = r;

			edge.push_back( e );
			return edge.size() - 1;
		} else {
			DEdge e;
			e.s = t;
			e.t = s;
			e.l = r;
			e.r = l;

			edge.push_back( e );
			return edge.size() - 1;
		}
	} else {
		return UNDEFINED;
	}
}

int Delaunay::findEdge(int s, int t){
	bool edgeExists = false;
	unsigned int i;

	for (i = 0; i < edge.size(); i++){
	    if ( (edge[i].s == s && edge[i].t == t) || (edge[i].s == t && edge[i].t == s)) {
			edgeExists = true;
			break;
	    }
	}

	if (edgeExists) return i;
	else return UNDEFINED;
}

void Delaunay::updateLeftFace(int eI, int s, int t, int f){
	if (!((edge[eI].s == s && edge[eI].t == t) || (edge[eI].s == t && edge[eI].t == s))){
		error( "updateLeftFace: adj. matrix and edge table mismatch" );
	}

	if (edge[eI].s == s && edge[eI].l == UNDEFINED){
		edge[eI].l = f;
	}else if (edge[eI].t == s && edge[eI].r == UNDEFINED){
		edge[eI].r = f;
	}else{
		error( "updateLeftFace: attempt to overwrite edge info" );
	}
}

} /* namespace OGLPool */
