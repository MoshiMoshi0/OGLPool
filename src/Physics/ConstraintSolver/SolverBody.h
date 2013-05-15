/*
 * SolverBody.h
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#ifndef SOLVERBODY_H_
#define SOLVERBODY_H_

#include <Entity/Entity.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
using namespace glm;

namespace OGLPool {
namespace Physics {

struct SolverBody {
public:
	SolverBody(){}
	SolverBody( Entity* e ) {
		originalEntity = e;
		vel = e->vel;
		angVel = e->angVel;
		pos = e->pos;
		rot = e->rot;
		massInv = e->massInv;
	}
	Entity*	originalEntity;

	vec3 m_deltaLinearVelocity;
	vec3 m_deltaAngularVelocity;
	vec3 m_pushVelocity;
	vec3 m_turnVelocity;
	vec3 vel;
	vec3 angVel;
	vec3 pos;
	quat rot;
	float massInv;

	void internalApplyImpulse(const vec3& linearComponent, const vec3& angularComponent,const float& impulseMagnitude){
		m_deltaLinearVelocity += linearComponent*impulseMagnitude;
		m_deltaAngularVelocity += angularComponent*impulseMagnitude;
	}

	void internalApplyPushImpulse(const vec3& linearComponent, const vec3& angularComponent,const float& impulseMagnitude) {
		m_pushVelocity += linearComponent*impulseMagnitude;
		m_turnVelocity += angularComponent*impulseMagnitude;
	}

	void writebackVelocity() {
		vel += m_deltaLinearVelocity;
		angVel += m_deltaAngularVelocity;
	}

	void integrateTranfsorm(const vec3& linvel, const vec3& angvel, float timeStep){
		pos += linvel * timeStep;
		vec3 axis;
		float	fAngle = angvel.length();
		//limit the angular motion
		if (fAngle*timeStep > 0.5f*half_pi<float>()) {
			fAngle = 0.5f*half_pi<float>() / timeStep;
		}

		if ( fAngle < float(0.001) ) {
			axis = angvel*( float(0.5)*timeStep-(timeStep*timeStep*timeStep)*(float(0.020833333333))*fAngle*fAngle );
		} else {
			axis = angvel*( sin(float(0.5)*fAngle*timeStep)/fAngle );
		}

		quat dorn (axis.x,axis.y,axis.z,cos( fAngle*timeStep*float(0.5) ));
		quat orn0 = rot;

		quat predictedOrn = dorn * orn0;
		rot = normalize(predictedOrn);
	}

	void writebackVelocityAndTransform(float timeStep, float splitImpulseTurnErp){
		if (originalEntity){
			vel += m_deltaLinearVelocity;
			angVel += m_deltaAngularVelocity;

			if (m_pushVelocity[0]!=0.f || m_pushVelocity[1]!=0 || m_pushVelocity[2]!=0 || m_turnVelocity[0]!=0 || m_turnVelocity[1]!=0 || m_turnVelocity[2]!=0){
				integrateTranfsorm( m_pushVelocity,m_turnVelocity*splitImpulseTurnErp, timeStep );
			}
		}
	}
};

}
} /* namespace OGLPool */
#endif /* SOLVERBODY_H_ */
