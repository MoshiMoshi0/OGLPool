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

	inline bool isNeighbor( int e ) const;
	inline bool connectsTo( const int& e ) const;

	int s,t;
	int l,r;
};

inline bool operator==( const DEdge& e0, const DEdge& e1 ){
	return (e0.s == e1.s && e0.t == e1.t) || (e0.s == e1.t && e0.t == e1.s);
}

bool DEdge::isNeighbor( int e ) const{
	return l == e || r == e;
}

bool DEdge::connectsTo( const int& e ) const{
	return s == e || t == e;
}

} /* namespace OGLPool */
#endif /* DEDGE_H_ */
