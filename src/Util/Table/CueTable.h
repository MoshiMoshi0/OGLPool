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

using namespace std;
using namespace glm;

namespace OGLPool {

class CueTable {
public:
	CueTable();
	CueTable(Polygon2 shape, vector<vec2> holePoints);
	virtual ~CueTable();
	void createTableMesh(Polygon2& shape);
	
	Mesh* tableMesh;
};

} /* namespace OGLPool */
#endif /* CUETABLE_H_ */

/* CUETABLE_H_ */
