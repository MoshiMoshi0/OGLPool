/*
 * SphereTriangleTester.cpp
 *
 *  Created on: 23-05-2013
 *      Author: _CORE7
 */

#include "SphereTriangleTester.h"

namespace OGLPool {

SphereTriangleTester::SphereTriangleTester(){}
SphereTriangleTester::SphereTriangleTester( Sphere* s, Triangle3* t ){ setBodies( s, t ); }

SphereTriangleTester::~SphereTriangleTester(){}

float SphereTriangleTester::closestToSegment( const vec3& from, const vec3& to, const vec3 &p, vec3 &closest ) {
	vec3 diff = p - from;
	vec3 v = to - from;
	float t = dot(v,diff);

	if (t > 0.0f) {
		float dotVV = dot(v,v);
		if (t < dotVV) {
			t /= dotVV;
			diff -= t*v;
		} else {
			t = 1.0f;
			diff -= v;
		}
	} else
		t = 0.0f;

	closest = from + t*v;
	return dot(diff, diff);
}

bool SphereTriangleTester::overlapTest( ContactManifold* manifold ){
	const vec3* vertices = t->getVertices();
	float radius = s->radius;

	vec3 normal = t->getNormal();
	vec3 p1ToCentre = s->pos - vertices[0];
	float distanceFromPlane = dot(p1ToCentre, normal);

	if (distanceFromPlane < 0.0f) {
		distanceFromPlane *= -1.0f;
		normal *= -1.0f;
	}

	bool isInsideContactPlane = distanceFromPlane < radius;
	bool hasContact = false;
	vec3 contactPoint;
	if (isInsideContactPlane) {
		if ( t->inside( s->pos )) {
			hasContact = true;
			contactPoint = s->pos - normal*distanceFromPlane;
		} else {
			float contactCapsuleRadiusSqr = radius*radius;
			vec3 nearestOnEdge;
			for (int i = 0; i < 3; i++) {
				Edge3 e = t->getEdge(i);

				float distanceSqr = closestToSegment(e[0], e[1], s->pos, nearestOnEdge);
				if (distanceSqr < contactCapsuleRadiusSqr) {
					hasContact = true;
					contactPoint = nearestOnEdge;
				}
			}
		}
	}

	if (hasContact) {
		vec3 contactToCentre = s->pos - contactPoint;
		float distanceSqr = dot( contactToCentre, contactToCentre );

		if (distanceSqr < radius*radius) {
			if (distanceSqr > epsilon<float>()) {
				float distance = sqrt( distanceSqr );
				manifold->addContact( contactPoint, normalize( contactToCentre ), -(radius-distance));
			} else {
				manifold->addContact( contactPoint, normal, -radius );
			}
			return true;
		}
	}

	return false;
}

bool SphereTriangleTester::sweptTest( ContactManifold* info ){
	assert(0);
	return false;
}

bool SphereTriangleTester::broadphase(){
	BoundingBox3 tb = BoundingBox3::get( {t->getVertex(0), t->getVertex(1), t->getVertex(2)}, 0.1f );
	return BoundingBox3::intersects( s->getBoundingBox(), tb );
}

void SphereTriangleTester::setBodies( Sphere* s, Triangle3* t ){ this->s = s; this->t = t; CollisionTester::setBodies( s, 0 ); }

} /* namespace OGLPool */
