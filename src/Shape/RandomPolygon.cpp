/*
 * RandomPolygon.cpp
 *
 *  Created on: 09-04-2013
 *      Author: _CORE7
 */

#include "RandomPolygon.h"
#include <Misc/Delaunay.h>
#include <glm/gtc/random.hpp>
#include <algorithm>
#include <iostream>
using namespace std;

namespace OGLPool {

RandomPolygon::RandomPolygon() {
	uint numSides = 16;
	uint numPoints = 20;

	Delaunay d;
	vector < vec2 > points;
	while( points.size() < numPoints ){ points.push_back( linearRand(vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f)) * 20.0f ); }
	d.triangulate(points);

	auto edges = d.getDEdges();
	auto boundary = getBoundary(edges);
	do {
		bool removed = false;
		if (boundary.size() > numSides) removed = removeEdges( 2, boundary, edges );
		if (boundary.size() < numSides || !removed) removed = removeEdges( 1, boundary, edges );

		if( removed ){
			removed = removeEdges( 3, boundary, edges ) | removed;
			boundary = getBoundary( edges );
		}

		if( !removed ) assert(0);
		if( numSides == boundary.size() ){
			if( removeEdges( 3, boundary, edges ) ){
				boundary = getBoundary( edges );
			}else{
				break;
			}
		}
	}while( true );

	for (uint i = 0; i < edges.size(); i++)
		this->addEdge( points[edges[i].s], points[edges[i].t] );
}

RandomPolygon::~RandomPolygon() {}

int RandomPolygon::getRandomTriIndex( vector<DEdge> edges ) {
	int randomIndex = rand() % edges.size();
	DEdge randomEdge = edges.at(randomIndex);
	int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;
	return triIndex;
}

bool RandomPolygon::removeEdges( int universeCount, vector<DEdge> boundary, vector<DEdge>& edges ){
	vector < DEdge > validEdges;

	copy_if(boundary.begin(), boundary.end(), back_inserter(validEdges),
		[=](DEdge edge) -> bool {
			//if( !edge.connectsTo( Delaunay::DEdgeType::UNIVERSE ) )
			//	assert( 0 );

			int triIndex = (edge.l == Delaunay::DEdgeType::UNIVERSE) ? edge.r : edge.l;
			return getUniverseEdgeCount(triIndex, boundary) == universeCount;
		}
	);

	if( validEdges.size() == 0 )
		return false;

	DEdge randomEdge = validEdges.at( rand() % validEdges.size() );
	int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;
	removeUniverseEdges( triIndex, edges );

	return true;
}


vector < DEdge > RandomPolygon::getBoundary( vector< DEdge > edges ){
	vector <DEdge > ret;

	for (uint i = 0; i < edges.size(); i++) {
		if (edges[i].isNeighbor( Delaunay::DEdgeType::UNIVERSE ))
			ret.push_back(edges[i]);
	}

	return ret;
}

int RandomPolygon::getUniverseEdgeCount( int triIndex, vector< DEdge > boundary ){
	int uCount = 0;

	for(uint i = 0; i < boundary.size(); i++){
		if(boundary[i].isNeighbor( triIndex ))
			uCount++;
	}

	return uCount;
}

void RandomPolygon::removeUniverseEdges( int triIndex, vector< DEdge >& edges ){
	edges.erase( remove_if( edges.begin(), edges.end(),
		[=](DEdge edge) -> bool {
			return edge.isNeighbor( Delaunay::DEdgeType::UNIVERSE ) && edge.isNeighbor( triIndex );
		}
	), edges.end() );

	for(uint i = 0; i < edges.size(); i++){
		if(edges[i].l == triIndex) edges[i].l = Delaunay::DEdgeType::UNIVERSE;
		else if (edges[i].r == triIndex) edges[i].r = Delaunay::DEdgeType::UNIVERSE;
	}
}

} /* namespace OGLPool */
