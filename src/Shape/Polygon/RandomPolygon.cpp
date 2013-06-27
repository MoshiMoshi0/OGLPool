/*
 * RandomPolygon.cpp
 *
 *  Created on: 09-04-2013
 *      Author: _CORE7
 */

#include "RandomPolygon.h"
#include <Util/Triangulation/Delaunay.h>
#include <glm/gtc/random.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <algorithm>
#include <map>
using namespace std;

namespace OGLPool {

RandomPolygon::RandomPolygon() {}
RandomPolygon::~RandomPolygon() {}

bool RandomPolygon::generate(uint numPoints, uint numSides, float scale, uint numTries) {
	Delaunay dt;
	vector<vec2> rndPoints;
	vector<DEdge> dtEdges;

	for (uint i = 0, valid = 0; !valid;) {
		rndPoints.clear();
		while (rndPoints.size() < numPoints) {
			rndPoints.push_back(linearRand(vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f)) * scale);
		}

		dt.triangulate(rndPoints);
		dtEdges = dt.getDEdges();

		valid = fixSides(dtEdges, numSides);
		if (valid) {
			valid &= validateBoundary(getBoundary(dtEdges), rndPoints, 60, 4);
		}

		if( i++ > numTries )
			return false;
	}
	buildPolygon(getBoundary(dtEdges), rndPoints);
	return true;
}

bool RandomPolygon::fixSides(vector<DEdge>& edges, uint numSides) {
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

void RandomPolygon::buildPolygon(const vector<DEdge>& boundary, const vector<vec2>& rndPoints) {
	vector<DEdge> sorted = sortBoundary( boundary );
	for (auto& edge : sorted) {
		addPoint( rndPoints[edge.s] );
		addEdge( Edge2(rndPoints[edge.s], rndPoints[edge.t]) );
	}
}

vector<DEdge> RandomPolygon::sortBoundary(const vector<DEdge>& boundary){
	vector<DEdge> ret;

	DEdge current = boundary[0];
	for (uint i = 0; i < boundary.size(); i++) {
		ret.push_back( current );

		DEdge next = getNextEdge( boundary, current );
		if (current.t == next.s) {
			current = next;
		} else {
			current = DEdge(next.t, next.s);
		}
	}

	return ret;
}

bool RandomPolygon::validateBoundary( const vector<DEdge>& boundary, const vector<vec2>& points, const float& minAng, const float& minLen ){
	vector<DEdge> sorted = sortBoundary( boundary );
	uint size = sorted.size();
	for( uint i = 0; i < size; i++ ){
		DEdge e0 = sorted.at( i );
		DEdge e1 = sorted.at( (i+1) % size );

		vec2 d0 = points[e0.s] - points[e0.t];
		vec2 d1 = points[e1.t] - points[e1.s];

		vec2 n0 = normalize(d0);
		vec2 n1 = normalize(d1);

		float len = length( d0 );
		float ang = angle( n0, n1 );

		if( ang < minAng ) return false;
		if( len < minLen ) return false;
	}

	return true;
}

DEdge RandomPolygon::getNextEdge(const vector<DEdge>& boundary, const DEdge& current) {
	for (uint j = 0; j < boundary.size(); j++) {
		DEdge next = boundary[j];

		if( next == current ) continue;
		if( next.connectsTo( current.t ) ) return next;
	}
	// This can't happen
	assert(0);
	return DEdge();
}

int RandomPolygon::getRandomTriIndex( const vector<DEdge>& edges ) {
	int randomIndex = rand() % edges.size();
	DEdge randomEdge = edges.at(randomIndex);
	return (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;
}

bool RandomPolygon::removeEdges( int universeCount, const vector<DEdge>& boundary, vector<DEdge>& edges ){
	vector<DEdge> validEdges;

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

	removeUniverseEdges( getRandomTriIndex( validEdges ), edges );

	return true;
}

bool RandomPolygon::canRemove( int triIndex, vector<DEdge> edges ){
	removeUniverseEdges( triIndex, edges );
	auto boundary = getBoundary( edges );
	map< int, int > pointCount;

	for( auto& edge : boundary ){
		pointCount[ edge.s ]++;
		pointCount[ edge.t ]++;
	}

	for( auto& pair : pointCount ){
		if( pair.second == 4 ){
			return false;
		}
	}

	return true;
}

vector<DEdge> RandomPolygon::getBoundary( const vector<DEdge>& edges ){
	vector <DEdge > ret;

	for( auto& edge : edges ){
		if (edge.isNeighbor( Delaunay::DEdgeType::UNIVERSE ))
			ret.push_back(edge);
	}

	return ret;
}

int RandomPolygon::getUniverseEdgeCount( int triIndex, const vector<DEdge>& boundary ){
	int uCount = 0;

	for( auto& edge : boundary ){
		if(edge.isNeighbor( triIndex ))
			uCount++;
	}

	return uCount;
}

void RandomPolygon::removeUniverseEdges( int triIndex, vector<DEdge>& edges ){
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
