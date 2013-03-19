/*
 * Entity.h
 *
 *  Created on: 17-03-2013
 *      Author: _CORE7
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

namespace OGLPool {

class Entity {
private:
	static int s_id;
public:
	Entity();
	Entity( vec3 pos );
	virtual ~Entity();

	virtual void render(){};
	virtual void update( float dt ){};
	virtual void applyTransform();
	virtual void applyForce( const vec3& f );
	virtual void applyOffsetForce( const vec3& f, const vec3& p );
	virtual void applyTorque( const vec3& f, const vec3& p );
	virtual void applyImpulse( const vec3& r, const vec3& i );

	virtual vec3 toWorldAxis( const vec3& v ) const;
	virtual vec3 toWorld( const vec3& v ) const;
	virtual mat3 toWorld( const mat3& mat ) const;

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
	virtual vec3 getVel() const;
	virtual void setVel(vec3 vel);

	float mass;
	float massInv;
	vec3 pos;
	vec3 vel;
	vec3 force;

	quat rot;
	vec3 angVel;
	vec3 torque;

	mat3 inertia;
	mat3 inertiaInv;
protected:
	int id;
};

} /* namespace OGLPool */
#endif /* ENTITY_H_ */
