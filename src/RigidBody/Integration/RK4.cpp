/*
 * RK4.cpp
 *
 *  Created on: 18-03-2013
 *      Author: _CORE7
 */

#include "RK4.h"

namespace OGLPool {

vec3 RK4::linAcc( RigidBody* e ){
	return e->force * e->massInv;
}

vec3 RK4::angAcc( RigidBody* e ){
	return e->inertiaInv * e->torque;
}

Derivative RK4::eval(RigidBody* e, float dt ){
	return Derivative( e->linVel, linAcc( e ), e->rot, angAcc( e ) );
}

Derivative RK4::eval(RigidBody* e, float dt, Derivative derivative ){
	vec3 v = e->linVel + derivative.acc * dt;

	quat rot = e->rot + derivative.spin * dt;
	vec3 av = e->angVel + derivative.angAcc * dt;

	quat spin = 0.5f * quat(0,av) * rot;
	return Derivative( v, linAcc( e ), spin, angAcc( e ) );
}

void RK4::integrate( RigidBody* e, float dt ){
	Derivative a = eval(e, 0.0f);
	Derivative b = eval(e, dt*0.5f, a);
	Derivative c = eval(e, dt*0.5f, b);
	Derivative d = eval(e, dt, c);

	vec3 vel = 1/6.f * ( a.vel + 2.0f * (b.vel + c.vel ) + d.vel );
	vec3 acc = 1/6.f * ( a.acc + 2.0f  * (b.acc + c.acc ) + d.acc );
	quat spin = 1/6.f * ( a.spin + 2.0f  * (b.spin + c.spin ) + d.spin );
	vec3 angAcc = 1/6.f * ( a.angAcc + 2.0f  * (b.angAcc + c.angAcc ) + d.angAcc );

	e->pos = e->pos + vel * dt;
	e->linVel = e->linVel + acc * dt;
	e->angVel = e->angVel + angAcc * dt;
	e->rot = normalize( e->rot + spin * dt );
}

} /* namespace OGLPool */
