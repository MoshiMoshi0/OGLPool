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
	DEdge();
	DEdge( int s, int t );
	DEdge( int s, int t, int l, int r );
	virtual ~DEdge(){};

	int s,t;
	int l,r;
};

} /* namespace OGLPool */
#endif /* DEDGE_H_ */
