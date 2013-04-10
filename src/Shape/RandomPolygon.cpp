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

	vector<vec2> points;
	Delaunay d;
	int numSides, numPts, cntEdges, triangle;
	cntEdges = 0;

	//numPts = (int) linearRand(float(20.0f), float(60.0f));
	//numSides = (int) linearRand(float(5.0f), float(20.0f));

	numPts = 21;
	numSides = 6;

	for (int i = 0; i < numPts; i++) {
		points.push_back(
				linearRand(vec2(-1.0f, -1.0f), vec2(1.0f, 1.0f)) * 20.0f);
	}
	d.triangulate(points);

	auto des = d.getDEdges();
	auto boundary = getBoundary( des );

	do{
		int randomIndex = rand() % boundary.size();
		DEdge randomEdge = boundary.at( randomIndex );

		int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;
		int universeEdgeCount = getTriangleUniverseEdgeCount( triIndex, boundary );

		//cout<<boundary.size()<<" " << triIndex << " " << universeEdgeCount << endl;


		bool removed = true;
		if(boundary.size() > numSides){
			vector< DEdge > x;
			copy_if( boundary.begin(), boundary.end(), x.begin(),
				[=](DEdge edge) -> bool {
					int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? edge.r : edge.l;
					return getTriangleUniverseEdgeCount( triIndex, boundary ) == 2;
				}
			);

			if( x.size() != 0 ){
				int randomIndex = rand() % x.size();
				DEdge randomEdge = x.at( randomIndex );
				int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;

				removeUniverseEdges( triIndex, universeEdgeCount, des );
				boundary = getBoundary( des );
			}else{
				removed = false;
			}
		}



		if( boundary.size() < numSides || !removed ){
			vector< DEdge > x;
			copy_if( boundary.begin(), boundary.end(), x.begin(),
				[=](DEdge edge) -> bool {
					int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? edge.r : edge.l;
					return getTriangleUniverseEdgeCount( triIndex, boundary ) == 1;
				}
			);

			int randomIndex = rand() % x.size();
			DEdge randomEdge = x.at( randomIndex );
			int triIndex = (randomEdge.l == Delaunay::DEdgeType::UNIVERSE) ? randomEdge.r : randomEdge.l;

			removeUniverseEdges( triIndex, universeEdgeCount, des );
			boundary = getBoundary( des );
		}
	}while( numSides != boundary.size() );


	/*for (int i = 0; i < des.size(); i++) {

		if (des[i].l == d.UNIVERSE || des[i].r == d.UNIVERSE) {
			cntEdges++;
		}
	}

	if (cntEdges < numSides) {
		for (int i = 0; cntEdges != numSides && i <= des.size();) {
			if (des[i].l == d.UNIVERSE) {
				triangle = des[i].r;
				des.erase(des.begin() + i);

				for (int j = 0; j < des.size(); j++) {
					if (des[i].r == triangle) {
						des[i].r = d.UNIVERSE;
					} else if (des[i].l == triangle) {
						des[i].l = d.UNIVERSE;
					}
				}
			} else if (des[i].r == d.UNIVERSE) {
				triangle = des[i].l;
				des.erase(des.begin() + i);
				for (int j = 0; j < des.size(); j++) {
					if (des[i].r == triangle) {
						des[i].r = d.UNIVERSE;
					} else if (des[i].l == triangle) {
						des[i].l = d.UNIVERSE;
					}
				}
			}
			i++;
		}
	} else if (cntEdges > numSides) {

	}*/

	for (int i = 0; i < des.size(); i++) {
		vec2 v0 = points[des[i].s];
		vec2 v1 = points[des[i].t];
		this->addEdge(v0, v1);
	}

}

RandomPolygon::~RandomPolygon() {
}

vector < DEdge > RandomPolygon::getBoundary( vector < DEdge > des ){
	vector <DEdge > bes;

	for (int i = 0; i < des.size(); i++) {
		if (des[i].l == Delaunay::DEdgeType::UNIVERSE || des[i].r == Delaunay::DEdgeType::UNIVERSE)
			bes.push_back(des[i]);
	}

	return bes;
}

int RandomPolygon::getTriangleUniverseEdgeCount( int triIndex, vector < DEdge > boundary ){
	int uCount = 0;

	for(int i = 0; i < boundary.size(); i++){
		if(boundary[i].r == triIndex || boundary[i].l == triIndex)
			uCount++;
	}

	return uCount;
}

void RandomPolygon::removeUniverseEdges( int triIndex, int universeEdgeCount, vector < DEdge >& des ){
	des.erase( remove_if( des.begin(), des.end(),
			[=](DEdge edge) -> bool {
				return (edge.l == Delaunay::DEdgeType::UNIVERSE || edge.r == Delaunay::DEdgeType::UNIVERSE) && (edge.l == triIndex || edge.r == triIndex);
			}
		), des.end() );

	for(int i = 0; i < des.size(); i++){
		if(des[i].l == triIndex){
			des[i].l = Delaunay::DEdgeType::UNIVERSE;
		}else if (des[i].r == triIndex){
			des[i].r = Delaunay::DEdgeType::UNIVERSE;
		}
	}
}

} /* namespace OGLPool */
