/*
 * Physics.h
 *
 *  Created on: 19-03-2013
 *      Author: _UX31A
 */

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <Physics/ContactInfo.h>
#include <Entity\Sphere.h>
#include <glm/gtc/constants.hpp>

namespace OGLPool {
	namespace Physics {
		bool sphereSphereIntersection( Sphere* s0, Sphere* s1 ){
			return length(s0->pos + s1->pos) <= s0->radius + s1->radius;
		}

		bool sphereRayIntersection( vec3 rayOrigin, vec3 rayDir, vec3 pos, float radius ){
			float a = dot(rayDir, rayDir);
			float b = 2 * dot(rayDir, rayOrigin);
			float c = dot(rayOrigin, rayOrigin) - radius * radius;

			float disc = b * b - 4 * a * c;

			if (disc < 0)
				return false;

			float distSqrt = sqrtf(disc);
			float q;
			if (b < 0) q = (-b - distSqrt)/2.0;
			else q = (-b + distSqrt)/2.0;

			float t0 = q / a;
			float t1 = c / q;

			if (t0 > t1) {
				float temp = t0;
				t0 = t1;
				t1 = temp;
			}

			if ( t1 < 0 )
				return false;

			return true;
		}

		bool sphereRayIntersection( vec3 rayOrigin, vec3 rayDir, Sphere* s ){
			return sphereRayIntersection( rayOrigin, rayDir, s->pos, s->radius );
		}

		bool sphereRayTest( vec3 rayOrigin, vec3 rayDir, vec3 pos, float radius, ContactInfo* info ){
			if( info == 0 )
				return false;

			vec3 d = rayOrigin - pos;
			float a = dot(rayDir, rayDir);
			float b = 2 * dot(rayDir, d);
			float c = dot(d, d) - radius * radius;

			float disc = b * b - 4 * a * c;

			if (disc < 0)
				return false;

			float distSqrt = sqrtf(disc);
			float q;
			if (b < 0) q = (-b - distSqrt)/2.0;
			else q = (-b + distSqrt)/2.0;

			float t0 = q / a;
			float t1 = c / q;

			if (t0 > t1) {
				float temp = t0;
				t0 = t1;
				t1 = temp;
			}

			if ( t1 < 0 )
				return false;

			float t = t0 < 0 ? t1 : t0;
			if( t > 0.15f || t < 0 ) return false;

			vec3 point = rayOrigin + rayDir * t;
			info->setContactTime( t );
			info->setContactPoint( point );
			info->setNormal( normalize( point - pos ) );
			return true;
		}

		bool spherePlaneIntersection(){
			return false;
		}

		bool spherePlaneTest( Sphere* s, Plane* p, ContactInfo* info ){
			float dist = dot( p->normal, s->getPos() ) - dot( p->normal, p->pos );
			if( fabs( dist ) <= s->radius ){
				info->setContactTime( 0 );
				info->setContactPoint( s->getPos() );
				info->setDepth( s->radius - fabs( dist ) );
				info->setNormal( p->normal );
				return false;
			}

			float denom = dot( p->normal, s->getVel() );
			if( denom * dist >= 0.0f ){
				return false;
			}

			float r = dist > 0.0f ? s->radius : -s->radius;
			float t = ( r - dist ) / denom;
			if( t > 0.15f || t < 0 ) return false;

			vec3 point = s->getPos() + t * s->getVel() - r * p->normal;

			info->setContactTime( t );
			info->setContactPoint( point );
			info->setNormal( p->normal );

			return true;
		}

		bool sphereRayTest( vec3 rayOrigin, vec3 rayDir, Sphere* s, ContactInfo* info ){
			return sphereRayTest( rayOrigin, rayDir, s->pos, s->radius, info );
		}

		bool sphereSphereTest( Sphere* s0, Sphere* s1, ContactInfo* info ){
			if( info == 0 ) return false;

			vec3 dir = s1->vel - s0->vel;
			return sphereRayTest( s1->pos, dir, s0->pos, s0->radius + s1->radius, info );
		}

		void response( Entity* e0, Entity* e1, ContactInfo* info ){
			e0->pos += e0->getVel() * info->getContactTime();
			e1->pos += e1->getVel() * info->getContactTime();

			vec3 n = info->getNormal();
			vec3 r0 = info->getContactPoint() - e0->pos;
			vec3 r1 = info->getContactPoint() - e1->pos;

			vec3 v0 = e0->getVel() + cross( r0, e0->angVel );
			vec3 v1 = e1->getVel() + cross( r1, e1->angVel );

			vec3 dv = v1 - v0;
			//if( dot( -dv,n ) < -0.01f ){
			//	return;
			//}

			float e = 0.5;
			float dn = e0->massInv + e1->massInv + dot( e0->getWorldInertiaInv() * cross(cross(r0,n),r0) + e1->getWorldInertiaInv() * cross(cross(r1,n),r1) ,n );
			float jn = 1*(1+e) * dot(dv,n) / dn;

			e0->applyImpulse( r0,  n * jn );
			e1->applyImpulse( r1, -n * jn );

			/*vec3 t = dv - dot(dv,n) * n;
			if( length(t) == 0 )
				return;
			dn = e0->massInv + e1->massInv + dot( e0->getWorldInertiaInv() * cross(cross(r0,t),r0) + e1->getWorldInertiaInv() * cross(cross(r1,t),r1), t );
			float jt = 1 * dot(dv,t) / dn;

			e0->vel += e0->massInv * t * jt;
			e0->angVel += e0->getWorldInertiaInv() * cross(r0,t * jt);

			e1->vel -= e1->massInv * t * jt;
			e1->angVel -= e1->getWorldInertiaInv() * cross(r1,t * jt);*/

			vec3 fe = e0->getForce() + e1->getForce();
			float vp = dot( dv,n );
			float fp = dot( fe,n );
			vec3 t = vec3();

			if( vp != 0 ){
				t = normalize( dv - vp * n );
			}else if( vp == 0 && fp != 0 ){
				t = normalize( fe - fp * n );
			}

			float us = 0.3f;
			float ud = 0.1f;
			float js = us * fabs( jn );
			float jd = ud * fabs( jn );

			float bm = (e0->mass + e1->mass) * dot( dv, t );
			if( bm <= js ){
				e0->applyImpulse( r0, -bm * t );
				e1->applyImpulse( r1, bm * t );
			}else{
				e0->applyImpulse( r0, -jd * t );
				e1->applyImpulse( r1, jd * t );
			}
		}

	} /* namespace Physics */
} /* namespace OGLPool */
#endif /* PHYSICS_H_ */
