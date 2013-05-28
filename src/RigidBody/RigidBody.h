/*
 * Entity.h
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#define SetBodyType( type ) \
virtual int getType(){ \
	return RigidBody::type; \
}

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <Shape/BoundingBox.h>
using namespace glm;

namespace OGLPool {

class RigidBody {
private:
	static int s_id;
public:
	enum {
		SPHERE, PLANE, MESH, UNKNOWN
	};

	SetBodyType( UNKNOWN );
	RigidBody();
	RigidBody( vec3 pos );
	RigidBody( RigidBody* e );
	virtual ~RigidBody();

	virtual void render(){};
	virtual void update( float dt ){};

	virtual void applyTransform();
	virtual void applyForce( const vec3& f );
	virtual void applyOffsetForce( const vec3& f, const vec3& p );
	virtual void applyTorque( const vec3& f, const vec3& p );
	virtual void applyImpulse( const vec3& r, const vec3& i );
	virtual void applyDamping( float dt );

	virtual vec3 toWorldAxis( const vec3& v ) const;
	virtual vec3 toWorld( const vec3& v ) const;
	virtual mat3 toWorld( const mat3& mat ) const;
	virtual vec3 toLocal( const vec3& v ) const;
	virtual vec3 toLocalAxis( const vec3& v ) const;

	virtual vec3 getAngVel() const;
	virtual void setAngVel(vec3 angVel);
	virtual vec3 getForce() const;
	virtual void setForce(vec3 force);
	virtual mat3 getInertia() const;
	virtual mat3 getWorldInertia() const;
	virtual void setInertia(mat3 inertia);
	virtual mat3 getWorldInertiaInv() const;
	virtual mat3 getInertiaInv() const;
	virtual float getMass() const;
	virtual void setMass(float mass);
	virtual float getMassInv() const;
	virtual vec3 getPos() const;
	virtual void setPos(vec3 pos);
	virtual quat getRot() const;
	virtual void setRot(quat rot);
	virtual vec3 getTorque() const;
	virtual void setTorque(vec3 torque);
	virtual vec3 getLinVel() const;
	virtual void setLinVel(vec3 vel);
	virtual const BoundingBox& getBoundingBox();
	virtual float getBoundingSphereRadius() const = 0;

	float mass;
	float massInv;

	vec3 pos;
	quat rot;

	vec3 linVel;
	vec3 angVel;

	vec3 force;
	vec3 torque;

	mat3 inertia;
	mat3 inertiaInv;

	float restitutionCoef;
	float frictionCoef;
	float rollingFrictionCoef;
	float linearDamping;
	float angularDamping;

	BoundingBox boundingBox;

	int id;
};

} /* namespace OGLPool */
#endif /* ENTITY_H_ */
