/*
 * CueTable.cpp
 *
 *  Created on: 20-06-2013
 *      Author: DarkNeo
 */

#include "CueTable.h"
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/gtx/compatibility.hpp>


using namespace std;
using namespace glm;


namespace OGLPool {

CueTable::CueTable(Polygon2 shape, int numOfHoles) {
	tableMesh = 0;
	addPockets(shape,numOfHoles);
	createTableMesh(shape);
}

CueTable::~CueTable() {}

void CueTable::addPockets(Polygon2& shape, int numOfHoles){
	vector< Edge2 > edges = shape.getEdges();
	for(int i = 0; i < numOfHoles; ){
		Edge2 edge = edges[rand() % edges.size()];
		float alpha = linearRand(0.0f, 1.0f);

		if(insertPocket(shape, lerp(edge[0],edge[1],alpha))){
			i++;
		}
	}
}

bool CueTable::insertPocket(Polygon2& shape, vec2 holePos){
	const Circle circle(holePos, 1.0f);
	auto edges = &shape.getEdges();
	int countEdges = 0;

	for(uint i = 0; i < edges->size(); i++){
		Edge2 edge = edges->at(i);
		if(circle.inside(edge[0]) ^ circle.inside(edge[1])){
			countEdges++;
		}
	}

	if(countEdges > 2){
		return false;
	}else if (countEdges == 1){
		return false;
	}

	for( auto it = edges->begin(); it != edges->end(); ){
		Edge2 edge = (*it);
		if(circle.inside(edge[0]) && circle.inside(edge[1])){
			it = edges->erase( it );
		}else{
			it++;
		}
	}

	for( auto it = edges->begin(); it != edges->end(); ){
		Edge2 edge = (*it);

		vec2 alpha;
		IntersectionType intType = circleIntersection(edge, circle, alpha.x, alpha.y);

		vec2 edge0 = edge[0];
		vec2 edge1 = edge[1];
		vec2 hit0 = lerp(edge0,edge1,alpha[0]);
		vec2 hit1 = lerp(edge0,edge1,alpha[1]);

		switch (intType) {
			case IMPALE: {
				it = edges->erase( it );
				it = edges->insert(it, Edge2(edge0, hit0));

				vector<Edge2> pocketEdges = generatePocketEdges(circle, Edge2(edge0, hit0), Edge2(hit1, edge1), 10);
				for( auto& e : pocketEdges )
					it = edges->insert( it + 1, e );

				it = edges->insert(it + 1, Edge2(hit1, edge1));
				it--;

				cout << "remove" << endl;
				break;
			}
			case POKE: {
				vector<Edge2> pocketEdges = generatePocketEdges(circle, *it, *(it+1), 10);
				it->at(1) = hit0;
				for( auto& e : pocketEdges )
					it = edges->insert( it + 1, e );

				break;
			}
			case EXITWOUND: {
				it->at(0) = hit1;
				break;
			}
			case COMPLETELYINSIDE:
			case FALLSHORT:
			case PAST:
			case NONE: break;
			case INVALID:
			default: assert(0);
		}

		it++;
	}

	return true;
}

vector <Edge2> CueTable::generatePocketEdges( const Circle& circle, const Edge2& startEdge, const Edge2& endEdge, int quality ){
	vector <Edge2> pocketEdges;

	float startAngle = orientedAngle(-startEdge.getDirection(),vec2(0,1));
	float endAngle = orientedAngle(endEdge.getDirection(), vec2(0,1));
	float startRad = radians( startAngle );
	//float endRad = radians( endAngle );

	float alpha = (endAngle - startAngle) / (float)(quality - 1);
	float alphaRad = radians( alpha );

	float tangetialFactor = tan( alphaRad );
	float radialFactor = cos( alphaRad );

	float x = circle.radius * cos( 0.0f );
	float y = circle.radius * sin( 0.0f );

	auto rotate = [=]( float x, float y) -> vec2 {
		vec3 i = vec3( x, 0, y );
		vec3 r = angleAxis(startAngle + 90, vec3(0,1,0) ) * i;
		return vec2( r.x, r.z );
	};

	for (int i = 0; i < quality - 1; i++){
		vec2 v0 = circle.center + rotate( x,y );

		float tx = -y;
		float ty = x;

		x += tx * tangetialFactor;
		y += ty * tangetialFactor;

		x *= radialFactor;
		y *= radialFactor;

		vec2 v1 = circle.center + rotate( x,y );
		pocketEdges.push_back( Edge2(v0, v1) );
	}

	return pocketEdges;
}

IntersectionType CueTable::circleIntersection(Edge2 edge, Circle circle, float& alpha0, float& alpha1){
	vec2 startEnd = edge[1] - edge[0];
	vec2 centerStart = edge[0] - circle.center;


	float a = dot(startEnd, startEnd);
	float b = 2 * dot(centerStart, startEnd);
	float c = dot(centerStart, centerStart) - circle.radius * circle.radius;

	float delta = b * b - 4 * a * c;

	if(delta < 0){
		return NONE;
	}else{
		delta = sqrt(delta);

		alpha0 = (-b - delta) / (2 * a);
		alpha1 = (-b + delta) / (2 * a);
	}

	if(alpha0 < 0){
		if(alpha1 > 1) return COMPLETELYINSIDE;
		if (alpha1 < 0) return PAST;
		if(alpha1 >= 0 && alpha1 <= 1) return EXITWOUND;
	}else if(alpha1 > 1){
		if(alpha0 > 1) return FALLSHORT;
		if(alpha0 >= 0 && alpha0 <= 1) return POKE;
	}

	if( (alpha1 >= 0 && alpha1 <= 1) && (alpha0 >= 0 && alpha0 <= 1) ) return IMPALE;
	return NONE;
}

void CueTable::createTableMesh(Polygon2& shape){
	tableMesh = new Mesh();
	
	float size = 2.0f;
	vec3 offset = vec3(0,1,0) * size;
	for(auto& edge : shape.getEdges()){
		vec3 v0 = vec3 (edge[0].x, 0,  edge[0].y);
		vec3 v1 = vec3 (edge[1].x, 0,  edge[1].y);
		vec3 v2 = v0 + offset;
		vec3 v3 = v1 + offset;
		
		tableMesh->beginTriangle();
			tableMesh->vertex(v2);
			tableMesh->vertex(v1);
			tableMesh->vertex(v0);

			tableMesh->color(vec3(0.0, 1.0, 0.0));
			tableMesh->color(vec3(0.0, 1.0, 0.0));
			tableMesh->color(vec3(0.0, 1.0, 0.0));

			tableMesh->calculateNormals();
		tableMesh->endTriangle();
		
		tableMesh->beginTriangle();
			tableMesh->vertex(v2);
			tableMesh->vertex(v3);
			tableMesh->vertex(v1);

			tableMesh->color(vec3(0.0, 1.0, 0.0));
			tableMesh->color(vec3(0.0, 1.0, 0.0));
			tableMesh->color(vec3(0.0, 1.0, 0.0));

			tableMesh->calculateNormals();
		tableMesh->endTriangle();
	}	
	
	tableMesh->build();
}

} /* namespace OGLPool */

