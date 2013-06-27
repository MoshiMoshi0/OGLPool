/*
 * CueTable.h
 *
 *  Created on: 20-06-2013
 *      Author: DarkNeo
 */

#ifndef CUETABLE_H_
#define CUETABLE_H_

#include <Shape/Polygon/Polygon.h>
#include <vector>
#include <glm/glm.hpp>
#include <RigidBody/Mesh.h>
#include <Shape/Circle.h>
#include <glm/gtx/vector_angle.hpp>

using namespace std;
using namespace glm;

namespace OGLPool {

enum IntersectionType{
	INVALID = -1,
	NONE,
	IMPALE,
	POKE,
	EXITWOUND,
	FALLSHORT,
	PAST,
	COMPLETELYINSIDE
};

class CueTable {
public:
	CueTable(Polygon2 shape, int numOfHoles);
	virtual ~CueTable();

	void addPockets(Polygon2& shape, int numOfHoles);
	bool insertPocket(Polygon2& shape, vec2 holePos);
	vector <Edge2> generatePocketEdges( const Circle& circle, const Edge2& startEdge, const Edge2& endEdge, int quality );
	void createTableMesh(Polygon2& shape);
	IntersectionType circleIntersection(Edge2 edge, Circle circle, float& alpha0, float& alpha1);
	
	Mesh* tableMesh;
	Polygon2 shape;
};

} /* namespace OGLPool */
#endif /* CUETABLE_H_ */

/* CUETABLE_H_ */
