/*
 * CueTable.cpp
 *
 *  Created on: 20-06-2013
 *      Author: DarkNeo
 */

#include "CueTable.h"
#include <iostream>
using namespace std;


namespace OGLPool {

CueTable::CueTable(Polygon2 shape, vector<vec2> holePoints) {
	tableMesh = 0;
	createTableMesh(shape);
}

CueTable::~CueTable() {}

void CueTable::createTableMesh(Polygon2& shape){
	tableMesh = new Mesh();
	
	float size = 5.0f;
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

