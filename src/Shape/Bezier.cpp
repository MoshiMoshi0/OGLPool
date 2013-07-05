/*
 * Bezier.cpp
 *
 *  Created on: 05-07-2013
 *      Author: _UX31A
 */

#include "Bezier.h"
#include <functional>
#include <iostream>
using namespace std;

namespace OGLPool {

template< class T >
Bezier<T>::Bezier() {}

template< class T >
Bezier<T>::Bezier( vector<T> controlPoints, uint quality ) {
	create( controlPoints, quality );
}

template< class T >
Bezier<T>::~Bezier() {}

template< class T >
void Bezier<T>::create( const vector<T>& controlPoints, uint quality ){
	function< uint(uint,uint) > binomial; binomial = [ &binomial ]( uint n, uint k ) -> uint {
		if( k == 0 || k == n ) return 1;
		if( k <= 0 || k >= n || n < 0 ) return 0;
		return binomial( n-1, k-1 ) * n / k;
	};

	const uint n = controlPoints.size() - 1;
	for( uint j = 0; j <= quality; j++ ){
		T point;
		float t = (float)(j) / quality;
		for( uint i = 0; i <= n; i++ ){
			float blend = binomial( n, i ) * powf( t, n - i ) * powf( 1.0f - t, i );
			point += blend * controlPoints[i];
		}
		points.push_back( point );
	}

	for (uint k = 0; k < points.size() - 1; k++) {
		edges.push_back( Edge<T>(points[k], points[k + 1]) );
	}
}

template class Bezier< vec2 >;
template class Bezier< vec3 >;

} /* namespace OGLPool */
