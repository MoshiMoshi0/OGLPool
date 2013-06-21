/*
 * Triangle.cpp
 *
 *  Created on: 22-03-2013
 *      Author: _CORE7
 */

#include "Triangle.h"

namespace OGLPool {

template< class T >
Triangle<T>::Triangle() {}
template< class T >
Triangle<T>::Triangle( T v0, T v1, T v2 ) {
	vertices[ 0 ] = v0; vertices[ 1 ] = v1; vertices[ 2 ] = v2;
	edges[0] = Edge<T>(v0,v1); edges[1] = Edge<T>(v1,v2); edges[2] = Edge<T>(v2,v0);
}

template< class T >
Triangle<T>::Triangle( Edge<T> e0, Edge<T> e1, Edge<T> e2 ) {
	vertices[ 0 ] = e0[0]; vertices[ 1 ] = e1[0]; vertices[ 2 ] = e2[0];
	edges[0] = e0; edges[1] = e1; edges[2] = e2;
}

template< class T >
Triangle<T>::~Triangle() {}

template< class T >
void Triangle<T>::render() const {
	for( int i = 0; i < 3; i++ ) edges[i].render();
}

template< class T >
Edge<T> Triangle<T>::getEdge( int i ){
	assert( i <= 2 );
	return edges[ i ];
}

template< class T >
T Triangle<T>::getVertex( int i ){
	assert( i <= 2 );
	return vertices[ i ];
}

template< class T >
T* Triangle<T>::getVertices(){
	return vertices;
}

template<>
vec3 Triangle<vec2>::getNormal(){
	return vec3(0.0f,0.0f,1.0f);
}

template<>
vec3 Triangle<vec3>::getNormal(){
	vec3 n0 = vertices[1] - vertices[0];
	vec3 n1 = vertices[2] - vertices[0];

	return normalize( cross( n0, n1 ) );
}

template< class T >
T& Triangle<T>::operator[](int i){
	return vertices[ i % 3 ];
}

template< class T >
T Triangle<T>::closestPoint( T source ){
	T edge0 = vertices[1] - vertices[0];
	T edge1 = vertices[2] - vertices[0];
	T v0 = vertices[0] - source;

	float a = dot( edge0, edge0 );
	float b = dot( edge0, edge1 );
	float c = dot( edge1, edge1 );
	float d = dot( edge0, v0 );
	float e = dot( edge1, v0 );

	float det = a*c - b*b;
	float s = b*e - c*d;
	float t = b*d - a*e;

	if ( s + t < det ) {
		if ( s < 0.f ) {
			if ( t < 0.f ) {
				if ( d < 0.f ) {
					s = clamp( -d/a, 0.f, 1.f );
					t = 0.f;
				} else {
					s = 0.f;
					t = clamp( -e/c, 0.f, 1.f );
				}
			} else {
				s = 0.f;
				t = clamp( -e/c, 0.f, 1.f );
			}
		} else if ( t < 0.f ) {
			s = clamp( -d/a, 0.f, 1.f );
			t = 0.f;
		} else {
			float invDet = 1.f / det;
			s *= invDet;
			t *= invDet;
		}
	} else {
		if ( s < 0.f ) {
			float tmp0 = b+d;
			float tmp1 = c+e;
			if ( tmp1 > tmp0 ) {
				float numer = tmp1 - tmp0;
				float denom = a-2*b+c;
				s = clamp( numer/denom, 0.f, 1.f );
				t = 1-s;
			} else {
				t = clamp( -e/c, 0.f, 1.f );
				s = 0.f;
			}
		} else if ( t < 0.f ) {
			if ( a+d > b+e ) {
				float numer = c+e-b-d;
				float denom = a-2*b+c;
				s = clamp( numer/denom, 0.f, 1.f );
				t = 1-s;
			} else {
				s = clamp( -e/c, 0.f, 1.f );
				t = 0.f;
			}
		} else {
			float numer = c+e-b-d;
			float denom = a-2*b+c;
			s = clamp( numer/denom, 0.f, 1.f );
			t = 1.f - s;
		}
	}

	return vertices[0] + s * edge0 + t * edge1;
}

template<>
bool Triangle<vec2>::inside( vec2 p ){
	assert(0);
	return false;
}

template<>
bool Triangle<vec3>::inside( vec3 p ){
	const vec3 p1 = vertices[0];
	const vec3 p2 = vertices[1];
	const vec3 p3 = vertices[2];
	const vec3 normal = getNormal();

	vec3 edge1( p2 - p1 );
	vec3 edge2( p3 - p2 );
	vec3 edge3( p1 - p3 );

	vec3 p1_to_p( p - p1 );
	vec3 p2_to_p( p - p2 );
	vec3 p3_to_p( p - p3 );

	vec3 edge1_normal( cross(edge1, normal));
	vec3 edge2_normal( cross(edge2, normal));
	vec3 edge3_normal( cross(edge3, normal));

	float r1, r2, r3;
	r1 = dot( edge1_normal, p1_to_p );
	r2 = dot( edge2_normal, p2_to_p );
	r3 = dot( edge3_normal, p3_to_p );
	if ( ( r1 > 0 && r2 > 0 && r3 > 0 ) ||
	     ( r1 <= 0 && r2 <= 0 && r3 <= 0 ) )
		return true;
	return false;

}

template class Triangle< vec2 >;
template class Triangle< vec3 >;

} /* namespace OGLPool */
