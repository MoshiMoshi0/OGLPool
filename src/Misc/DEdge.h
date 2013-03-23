/*
 * DEdge.h
 *
 *  Created on: 23-03-2013
 *      Author: _CORE7
 */

#ifndef DEDGE_H_
#define DEDGE_H_

namespace OGLPool {

class DEdge {
public:
	DEdge() { s = t = 0; }
	DEdge(int s, int t) { this->s = s; this->t = t; }
	virtual ~DEdge(){};

	int s,t;
	int l,r;
};

} /* namespace OGLPool */
#endif /* DEDGE_H_ */
