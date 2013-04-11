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
#include <map>
#include <iostream>
using namespace std;

namespace OGLPool {

RandomPolygon::RandomPolygon() {
	uint numSides = 16;
	uint numPoints = 20;

	Delaunay d;
	vector < vec2 > points;

	vector<DEdge> edges;
	bool valid = false;
	for( int i = 0; !valid; i++ ){
		points.clear();
		while (points.size() < numPoints) { points.push_back(linearRand(vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f)) * 20.0f); }

		d.triangulate(points);
		edges = d.getDEdges();

		valid = generate(edges, numSides);
		assert( i < 100 );
	}

	auto boundary = getBoundary( edges );
	for (uint i = 0; i < boundary.size(); i++)
		this->addEdge( points[boundary[i].s], points[boundary[i].t] );
}

RandomPolygon::~RandomPolygon() {}

bool RandomPolygon::generate(vector< DEdge >& edges, uint numSides) {
	auto boundary = getBoundary(edges);
	do {
		bool removed = false;
		if (boundary.size() > numSides)
			removed = removeEdges(2, boundary, edges);

		if (boundary.size() < numSides || !removed)
			removed = removeEdges(1, boundary, edges);

		if (removed)
			boundary = getBoundary(edges);

		if (numSides == boundary.size()) {
			break;
		} else if (!removed) {
			return false;
		}
	} while (true);

	return true;
}

int RandomPolygon::getRandomTriIndex( const vector< DEdge >& edges ) {
	int randomIndex = rand() % edges.size();
	DEdge randomEdge = edges.at(randomIndex);
	int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;
	return triIndex;
}

bool RandomPolygon::removeEdges( int universeCount, const vector<DEdge>& boundary, vector<DEdge>& edges ){
	vector < DEdge > validEdges;

	copy_if(boundary.begin(), boundary.end(), back_inserter(validEdges),
		[=](DEdge edge) -> bool {
			int triIndex = (edge.l == Delaunay::DEdgeType::UNIVERSE) ? edge.r : edge.l;
			bool isValid = getUniverseEdgeCount(triIndex, boundary) == universeCount;

			if( isValid && universeCount == 1 ){
				isValid &= canRemove( triIndex, edges );
			}
			return isValid;
		}
	);

	if( validEdges.size() == 0 )
		return false;

	DEdge randomEdge = validEdges.at( rand() % validEdges.size() );
	int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;
	removeUniverseEdges( triIndex, edges );

	return true;
}

bool RandomPolygon::canRemove( int triIndex, vector<DEdge> edges ){
	removeUniverseEdges( triIndex, edges );
	auto boundary = getBoundary( edges );
	map< int, int > pointCount;

	for( auto& edge : boundary ){
		if( pointCount.find( edge.s ) == pointCount.end() ) pointCount[ edge.s ] = 1;
		else pointCount[ edge.s ]++;

		if( pointCount.find( edge.t ) == pointCount.end() ) pointCount[ edge.t ] = 1;
		else pointCount[ edge.t ]++;
	}

	for( auto& pair : pointCount ){
		if( pair.second == 4 ){
			return false;
		}
	}

	return true;
}

vector < DEdge > RandomPolygon::getBoundary( const vector< DEdge >& edges ){
	vector <DEdge > ret;

	for( auto& edge : edges ){
		if (edge.isNeighbor( Delaunay::DEdgeType::UNIVERSE ))
			ret.push_back(edge);
	}

	return ret;
}

int RandomPolygon::getUniverseEdgeCount( int triIndex, const vector< DEdge >& boundary ){
	int uCount = 0;

	for( auto& edge : boundary ){
		if(edge.isNeighbor( triIndex ))
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

	for( auto& edge : edges ){
		if(edge.l == triIndex) edge.l = Delaunay::DEdgeType::UNIVERSE;
		else if (edge.r == triIndex) edge.r = Delaunay::DEdgeType::UNIVERSE;
	}
}

} /* namespace OGLPool */
