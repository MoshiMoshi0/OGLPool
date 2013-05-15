/*
 * ImpulseConstraintSolver.cpp
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#include "ImpulseConstraintSolver.h"
#include <algorithm>
#include <iostream>
using namespace std;

namespace OGLPool {
namespace Physics {

ImpulseConstraintSolver::ImpulseConstraintSolver(){}

ImpulseConstraintSolver::~ImpulseConstraintSolver(){
	solverBodyPool.erase( remove_if( solverBodyPool.begin(), solverBodyPool.end(),
		[](SolverBody* element) -> bool {
			delete element;
			return true;
		}
	), solverBodyPool.end() );
}

void planeSpace( const vec3& n, vec3& t0, vec3& t1 ){
	if( fabs( n.z ) > sqrtf( 1/2 ) ){
		float a = n.y * n.y + n.z * n.z;
		float k = 1.0f/sqrt( a );

		t0 = vec3(0, -n.z * k, n.y * k);
		t1 = vec3(a*k, -n.x * t0.z, n.x * t0.y);
	}else{
		float a = n.x * n.x + n.y * n.y;
		float k = 1.0f/sqrt( a );

		t0 = vec3(-n.y*k, n.x * k, 0);
		t1 = vec3(-n.z * t0.y, n.z * t0.x, a*k);
	}
}

void ImpulseConstraintSolver::setupContactConstraint( SolverConstraint& constraint, SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, float& relaxation, vec3& vel, float& rel_vel ){
	Entity* rb0 = sb0->originalEntity;
	Entity* rb1 = sb1->originalEntity;

	relaxation = 1.0f;

	vec3 torqueAxis0 = cross(r0, n );
	constraint.m_angularComponentA = rb0->getWorldInertiaInv()*torqueAxis0;
	vec3 torqueAxis1 = cross(r1, n);
	constraint.m_angularComponentB = rb1->getWorldInertiaInv()*-torqueAxis1;

	vec3 vec;
	float denom0 = 0.f;
	float denom1 = 0.f;
	if (rb0){
		vec = cross(constraint.m_angularComponentA,r0);
		denom0 = rb0->massInv + dot(n,vec);
	}
	if (rb1){
		vec = cross(-constraint.m_angularComponentB,r1);
		denom1 = rb1->massInv + dot(n,vec);
	}

	float denom = relaxation/(denom0+denom1);
	constraint.m_jacDiagABInv = denom;

	constraint.m_contactNormal = n;
	constraint.m_relpos1CrossNormal = torqueAxis0;
	constraint.m_relpos2CrossNormal = -torqueAxis1;

	float restitution = 0.f;
	float penetration = info->depth + infoGlobal.m_linearSlop;

	vec3 vel1,vel2;

	vel1 = rb0->vel + cross( rb0->angVel, r0 );
	vel2 = rb1->vel + cross( rb1->angVel, r1 );

	vel  = vel1 - vel2;
	rel_vel = dot(n,vel);

	constraint.m_friction = info->m_combinedFriction;

	restitution = info->m_combinedRestitution * -rel_vel;
	if (restitution <= float(0.)){
		restitution = 0.f;
	}

	if (infoGlobal.m_solverMode & SOLVER_USE_WARMSTARTING){
		constraint.m_appliedImpulse = info->m_appliedImpulse * infoGlobal.m_warmstartingFactor;

		sb0->internalApplyImpulse( constraint.m_contactNormal*sb0->massInv, constraint.m_angularComponentA, constraint.m_appliedImpulse);
		sb1->internalApplyImpulse( constraint.m_contactNormal*sb1->massInv, -constraint.m_angularComponentB, -constraint.m_appliedImpulse);
	} else {
		constraint.m_appliedImpulse = 0.f;
	}

	constraint.m_appliedPushImpulse = 0.f;

	{
		float vel1Dotn = dot( constraint.m_contactNormal, sb0->vel) + dot(constraint.m_relpos1CrossNormal, sb0->angVel);
		float vel2Dotn = -dot(constraint.m_contactNormal, sb1->vel) + dot(constraint.m_relpos2CrossNormal, sb1->angVel);
		float rel_vel = vel1Dotn+vel2Dotn;

		float positionalError = 0.f;
		float velocityError = restitution - rel_vel;// * damping;

		float erp = infoGlobal.m_erp2;
		if (!infoGlobal.m_splitImpulse || (penetration > infoGlobal.m_splitImpulsePenetrationThreshold)){
			erp = infoGlobal.m_erp;
		}

		if (penetration>0){
			positionalError = 0;

			velocityError -= penetration / infoGlobal.m_timeStep;
		} else {
			positionalError = -penetration * erp/infoGlobal.m_timeStep;
		}

		float  penetrationImpulse = positionalError*constraint.m_jacDiagABInv;
		float velocityImpulse = velocityError *constraint.m_jacDiagABInv;

		if (!infoGlobal.m_splitImpulse || (penetration > infoGlobal.m_splitImpulsePenetrationThreshold)){
			constraint.m_rhs = penetrationImpulse+velocityImpulse;
			constraint.m_rhsPenetration = 0.f;

		} else{
			constraint.m_rhs = velocityImpulse;
			constraint.m_rhsPenetration = penetrationImpulse;
		}
		constraint.m_cfm = 0.0f;
		constraint.m_lowerLimit = 0;
		constraint.m_upperLimit = 1e10f;
	}

	contactConstraintPool.push_back( constraint );
}

void ImpulseConstraintSolver::addFrictionConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, uint frictionIndex, float& relaxation ){
	SolverConstraint constraint(sb0, sb1);
	constraint.m_frictionIndex = frictionIndex;

	constraint.m_contactNormal = n;

	Entity* body0 = sb0->originalEntity;
	Entity* body1 = sb1->originalEntity;

	constraint.m_friction = info->m_combinedFriction;
	constraint.m_originalContactPoint = 0;

	constraint.m_appliedImpulse = 0.f;
	constraint.m_appliedPushImpulse = 0.f;

	{
		vec3 ftorqueAxis1 = cross(r0,constraint.m_contactNormal);
		constraint.m_relpos1CrossNormal = ftorqueAxis1;
		constraint.m_angularComponentA = body0->getWorldInertiaInv()*ftorqueAxis1;
	}
	{
		vec3 ftorqueAxis1 = cross(r1,-constraint.m_contactNormal);
		constraint.m_relpos2CrossNormal = ftorqueAxis1;
		constraint.m_angularComponentB = body1->getWorldInertiaInv()*ftorqueAxis1;
	}

	vec3 vec;
	float denom0 = 0.f;
	float denom1 = 0.f;
	if (body0) {
		vec = cross(constraint.m_angularComponentA,r0);
		denom0 = body0->massInv + dot(n,vec);
	}
	if (body1) {
		vec = cross(-constraint.m_angularComponentB,r1);
		denom1 = body1->massInv + dot(n,vec);
	}
	float denom = relaxation/(denom0+denom1);
	constraint.m_jacDiagABInv = denom;

	float rel_vel;
	float vel1Dotn = dot(constraint.m_contactNormal,sb0->vel)+ dot(constraint.m_relpos1CrossNormal,sb0->angVel);
	float vel2Dotn = -dot(constraint.m_contactNormal,sb1->vel)+ dot(constraint.m_relpos2CrossNormal,sb1->angVel);

	rel_vel = vel1Dotn+vel2Dotn;

	float velocityError =  0.0f - rel_vel;
	float velocityImpulse = velocityError * constraint.m_jacDiagABInv;
	constraint.m_rhs = velocityImpulse;
	constraint.m_cfm = 0.0f;
	constraint.m_lowerLimit = 0;
	constraint.m_upperLimit = 1e10f;

	frictionConstraintPool.push_back( constraint );
}

void ImpulseConstraintSolver::addRollingFrictionConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, uint frictionIndex, float& relaxation ){
	SolverConstraint constraint(sb0, sb1);
	constraint.m_frictionIndex = frictionIndex;

	constraint.m_contactNormal = vec3();

	Entity* body0 = sb0->originalEntity;
	Entity* body1 = sb1->originalEntity;

	constraint.m_friction = info->m_combinedRollingFriction;
	constraint.m_originalContactPoint = 0;

	constraint.m_appliedImpulse = 0.f;
	constraint.m_appliedPushImpulse = 0.f;

	{
		vec3 ftorqueAxis1 = -n;
		constraint.m_relpos1CrossNormal = ftorqueAxis1;
		constraint.m_angularComponentA = body0->getWorldInertiaInv()*ftorqueAxis1;
	}
	{
		vec3 ftorqueAxis1 = n;
		constraint.m_relpos2CrossNormal = ftorqueAxis1;
		constraint.m_angularComponentB = body1->getWorldInertiaInv()*ftorqueAxis1;
	}

	vec3 iMJaA = body0->getWorldInertiaInv()*constraint.m_relpos1CrossNormal;
	vec3 iMJaB = body1->getWorldInertiaInv()*constraint.m_relpos2CrossNormal;
	float sum = 0;
	sum += dot(iMJaA, constraint.m_relpos1CrossNormal);
	sum += dot(iMJaB, constraint.m_relpos2CrossNormal);
	constraint.m_jacDiagABInv = 1.0f/sum;

	float vel1Dotn = dot(constraint.m_contactNormal, sb0->vel) + dot(constraint.m_relpos1CrossNormal, sb0->angVel);
	float vel2Dotn = -dot(constraint.m_contactNormal, sb1->vel)+ dot(constraint.m_relpos2CrossNormal, sb1->angVel);

	float rel_vel = vel1Dotn+vel2Dotn;

	float velocityError =  0.0f - rel_vel;
	float	velocityImpulse = velocityError * float(constraint.m_jacDiagABInv);
	constraint.m_rhs = velocityImpulse;
	constraint.m_cfm = 0.0f;
	constraint.m_lowerLimit = 0;
	constraint.m_upperLimit = 1e10f;

	rollingFrictionConstraintPool.push_back( constraint );
}

SolverBody* ImpulseConstraintSolver::getSolverBody( Entity* e ){
	if( e ){
		for( auto& sb : solverBodyPool ){
			if( sb && sb->originalEntity == e ){
				return sb;
			}
		}
	}

	SolverBody* solverBody = 0;
	if( e ){
		solverBody = new SolverBody( e );
	}else{
		solverBody = new SolverBody();

		solverBody->originalEntity = 0;
		solverBody->massInv = 0;
		solverBody->vel = vec3();
		solverBody->angVel = vec3();
		solverBody->pos = vec3();
		solverBody->rot = quat();
	}

	solverBody->m_deltaLinearVelocity = vec3();
	solverBody->m_deltaAngularVelocity = vec3();
	solverBody->m_pushVelocity = vec3();
	solverBody->m_turnVelocity = vec3();
	return solverBody;
}

void ImpulseConstraintSolver::setFrictionConstraintImpulse( SolverConstraint& constraint, SolverBody* sb0, SolverBody* sb1, ContactInfo* info ){
	SolverConstraint& frictionConstraint1 = frictionConstraintPool[constraint.m_frictionIndex];
	if (infoGlobal.m_solverMode & SOLVER_USE_WARMSTARTING) {
		frictionConstraint1.m_appliedImpulse = info->m_appliedImpulseLateral1 * infoGlobal.m_warmstartingFactor;
		if (sb0) sb0->internalApplyImpulse(frictionConstraint1.m_contactNormal*sb0->massInv,frictionConstraint1.m_angularComponentA,frictionConstraint1.m_appliedImpulse);
		if (sb1) sb1->internalApplyImpulse(frictionConstraint1.m_contactNormal*sb1->massInv,-frictionConstraint1.m_angularComponentB,-frictionConstraint1.m_appliedImpulse);
	} else {
		frictionConstraint1.m_appliedImpulse = 0.f;
	}


	if ((infoGlobal.m_solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)){
		SolverConstraint& frictionConstraint2 = frictionConstraintPool[constraint.m_frictionIndex+1];
		if (infoGlobal.m_solverMode & SOLVER_USE_WARMSTARTING) {
			frictionConstraint2.m_appliedImpulse = info->m_appliedImpulseLateral2  * infoGlobal.m_warmstartingFactor;
			if (sb0) sb0->internalApplyImpulse(frictionConstraint2.m_contactNormal*sb0->massInv,frictionConstraint2.m_angularComponentA,frictionConstraint2.m_appliedImpulse);
			if (sb1) sb1->internalApplyImpulse(frictionConstraint2.m_contactNormal*sb1->massInv,-frictionConstraint2.m_angularComponentB,-frictionConstraint2.m_appliedImpulse);
		} else {
			frictionConstraint2.m_appliedImpulse = 0.f;
		}
	}
}

void ImpulseConstraintSolver::convertContact( ContactInfo* info ){
	Entity* e0 = info->e0;
	Entity* e1 = info->e1;
	SolverBody* sb0 = getSolverBody( e0 );
	SolverBody* sb1 = getSolverBody( e1 );

	assert( sb0 != sb1 );

	vec3 n = info->normal;
	vec3 r0 = info->point0 - e0->pos;
	vec3 r1 = info->point1 - e1->pos;

	float relaxation;
	float rel_vel;
	vec3 vel;

	uint frictionIndex = frictionConstraintPool.size();
	SolverConstraint constraint( sb0, sb1 );
	constraint.m_originalContactPoint = info;
	constraint.m_frictionIndex = frictionIndex;

	setupContactConstraint( constraint, sb0, sb1, r0, r1, n, info, relaxation, vel, rel_vel );

	vec3 da = sb1->angVel - sb0->angVel;
	if( info->m_combinedRollingFriction > 0.f){
		if( length( da ) > infoGlobal.m_singleAxisRollingFrictionThreshold ){
			da = normalize( da );
			if ( length(da) > 0.001f ) addRollingFrictionConstraint( sb0, sb1, r0, r1, da, info, frictionIndex, relaxation );
		}else{
			vec3 t0,t1;
			planeSpace( n, t0, t1 );

			addRollingFrictionConstraint( sb0, sb1, r0, r1, n, info, frictionIndex, relaxation );
			if ( length( t0 ) > 0.001f ) addRollingFrictionConstraint( sb0, sb1, r0, r1, t0, info, frictionIndex, relaxation );
			if ( length( t1 ) > 0.001f ) addRollingFrictionConstraint( sb0, sb1, r0, r1, t1, info, frictionIndex, relaxation );
		}
	}

	if (!(infoGlobal.m_solverMode & SOLVER_ENABLE_FRICTION_DIRECTION_CACHING) || !info->m_lateralFrictionInitialized ){
		info->m_lateralFrictionDir1 = vel - n * rel_vel;
		float lat_rel_vel = dot( info->m_lateralFrictionDir1, info->m_lateralFrictionDir1 );
		if( !(infoGlobal.m_solverMode & SOLVER_DISABLE_VELOCITY_DEPENDENT_FRICTION_DIRECTION) && lat_rel_vel > epsilon<float>() ){
			info->m_lateralFrictionDir1 *= 1.f/sqrt(lat_rel_vel);
			if((infoGlobal.m_solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)) {
				info->m_lateralFrictionDir2 = normalize( cross( info->m_lateralFrictionDir1, n ) );
				addFrictionConstraint( sb0, sb1, r0, r1, info->m_lateralFrictionDir2, info, frictionIndex, relaxation );
			}
			addFrictionConstraint( sb0, sb1, r0, r1, info->m_lateralFrictionDir1, info, frictionIndex, relaxation );
		}else{
			planeSpace(n,info->m_lateralFrictionDir1,info->m_lateralFrictionDir2);

			if ((infoGlobal.m_solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)){
				addFrictionConstraint( sb0, sb1, r0, r1, info->m_lateralFrictionDir2, info, frictionIndex, relaxation );
			}

			addFrictionConstraint( sb0, sb1, r0, r1, info->m_lateralFrictionDir1, info, frictionIndex, relaxation );
			if ((infoGlobal.m_solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS) && (infoGlobal.m_solverMode & SOLVER_DISABLE_VELOCITY_DEPENDENT_FRICTION_DIRECTION)) {
				info->m_lateralFrictionInitialized = true;
			}
		}
	} else {
		addFrictionConstraint( sb0, sb1, r0, r1, info->m_lateralFrictionDir1, info, frictionIndex, relaxation );
		if ((infoGlobal.m_solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS))
			addFrictionConstraint( sb0, sb1, r0, r1, info->m_lateralFrictionDir2, info, frictionIndex, relaxation );

		setFrictionConstraintImpulse( constraint, sb0, sb1, info );
	}
}

void ImpulseConstraintSolver::resolveSplitPenetrationImpulseCacheFriendly( SolverConstraint& c ){
	SolverBody* body1 = c.sb0;
	SolverBody* body2 = c.sb1;

	if (c.m_rhsPenetration) {
		float deltaImpulse = c.m_rhsPenetration-c.m_appliedPushImpulse*c.m_cfm;
		float deltaVel1Dotn =  dot(c.m_contactNormal, body1->m_pushVelocity) + dot( c.m_relpos1CrossNormal, body1->m_turnVelocity );
		float deltaVel2Dotn = -dot(c.m_contactNormal, body2->m_pushVelocity) + dot( c.m_relpos1CrossNormal, body2->m_turnVelocity );

		deltaImpulse -= deltaVel1Dotn*c.m_jacDiagABInv;
		deltaImpulse -= deltaVel2Dotn*c.m_jacDiagABInv;
		float sum = float(c.m_appliedPushImpulse) + deltaImpulse;
		if (sum < c.m_lowerLimit) {
			deltaImpulse = c.m_lowerLimit-c.m_appliedPushImpulse;
			c.m_appliedPushImpulse = c.m_lowerLimit;
		} else {
			c.m_appliedPushImpulse = sum;
		}

		body1->internalApplyPushImpulse(c.m_contactNormal*body1->massInv,c.m_angularComponentA,deltaImpulse);
		body2->internalApplyPushImpulse(-c.m_contactNormal*body2->massInv,c.m_angularComponentB,deltaImpulse);
	}
}

void ImpulseConstraintSolver::solveSingleIteration( int iteration, const vector< Entity* >& bodies, const vector< ContactInfo* >& infos ){
	if (iteration >= infoGlobal.m_numIterations) assert(0);

	uint numPoolConstraints = contactConstraintPool.size();
	for (uint j=0;j<numPoolConstraints;j++){
		SolverConstraint& solveManifold = contactConstraintPool[j];
		resolveSingleConstraintRowLowerLimit(solveManifold);
	}

	uint numFrictionPoolConstraints = frictionConstraintPool.size();
	for (uint j=0;j<numFrictionPoolConstraints;j++){
		SolverConstraint& solveManifold = frictionConstraintPool[j];
		float totalImpulse = contactConstraintPool[solveManifold.m_frictionIndex].m_appliedImpulse;

		if (totalImpulse > 0) {
			solveManifold.m_lowerLimit = -(solveManifold.m_friction*totalImpulse);
			solveManifold.m_upperLimit = solveManifold.m_friction*totalImpulse;

			resolveSingleConstraintRowGeneric(solveManifold);
		}
	}

	uint numRollingFrictionPoolConstraints = rollingFrictionConstraintPool.size();
	for (uint j=0;j<numRollingFrictionPoolConstraints;j++){
		SolverConstraint& rollingFrictionConstraint = rollingFrictionConstraintPool[j];
		float totalImpulse = contactConstraintPool[rollingFrictionConstraint.m_frictionIndex].m_appliedImpulse;
		if (totalImpulse > 0){
			float rollingFrictionMagnitude = rollingFrictionConstraint.m_friction*totalImpulse;
			if (rollingFrictionMagnitude>rollingFrictionConstraint.m_friction)
				rollingFrictionMagnitude = rollingFrictionConstraint.m_friction;

			rollingFrictionConstraint.m_lowerLimit = -rollingFrictionMagnitude;
			rollingFrictionConstraint.m_upperLimit = rollingFrictionMagnitude;

			resolveSingleConstraintRowGeneric(rollingFrictionConstraint);
		}
	}
}

void ImpulseConstraintSolver::resolveSingleConstraintRowLowerLimit(SolverConstraint& c){
	SolverBody* body1 = c.sb0;
	SolverBody* body2 = c.sb1;

	float deltaImpulse = c.m_rhs-c.m_appliedImpulse*c.m_cfm;
	const float deltaVel1Dotn =	 dot(c.m_contactNormal,body1->m_deltaLinearVelocity) + dot(c.m_relpos1CrossNormal,body1->m_deltaAngularVelocity);
	const float deltaVel2Dotn = -dot(c.m_contactNormal,body2->m_deltaLinearVelocity) + dot(c.m_relpos2CrossNormal,body2->m_deltaAngularVelocity);

	deltaImpulse -= deltaVel1Dotn*c.m_jacDiagABInv;
	deltaImpulse -= deltaVel2Dotn*c.m_jacDiagABInv;
	const float sum = c.m_appliedImpulse + deltaImpulse;
	if (sum < c.m_lowerLimit){
		deltaImpulse = c.m_lowerLimit-c.m_appliedImpulse;
		c.m_appliedImpulse = c.m_lowerLimit;
	}else{
		c.m_appliedImpulse = sum;
	}

	body1->internalApplyImpulse(c.m_contactNormal*body1->massInv,c.m_angularComponentA,deltaImpulse);
	body2->internalApplyImpulse(-c.m_contactNormal*body2->massInv,c.m_angularComponentB,deltaImpulse);
}

void ImpulseConstraintSolver::resolveSingleConstraintRowGeneric(SolverConstraint& c){
	SolverBody* body1 = c.sb0;
	SolverBody* body2 = c.sb1;

	float deltaImpulse = c.m_rhs-c.m_appliedImpulse*c.m_cfm;
	float deltaVel1Dotn =  dot(c.m_contactNormal, body1->m_deltaLinearVelocity) + dot(c.m_relpos1CrossNormal, body1->m_deltaAngularVelocity);
	float deltaVel2Dotn = -dot(c.m_contactNormal, body2->m_deltaLinearVelocity) + dot(c.m_relpos2CrossNormal, body2->m_deltaAngularVelocity);

	deltaImpulse -= deltaVel1Dotn*c.m_jacDiagABInv;
	deltaImpulse -= deltaVel2Dotn*c.m_jacDiagABInv;

	float sum = c.m_appliedImpulse + deltaImpulse;
	if (sum < c.m_lowerLimit) {
		deltaImpulse = c.m_lowerLimit-c.m_appliedImpulse;
		c.m_appliedImpulse = c.m_lowerLimit;
	} else if (sum > c.m_upperLimit)  {
		deltaImpulse = c.m_upperLimit-c.m_appliedImpulse;
		c.m_appliedImpulse = c.m_upperLimit;
	} else {
		c.m_appliedImpulse = sum;
	}

	body1->internalApplyImpulse(c.m_contactNormal*body1->massInv,c.m_angularComponentA,deltaImpulse);
	body2->internalApplyImpulse(-c.m_contactNormal*body2->massInv,c.m_angularComponentB,deltaImpulse);
}

void ImpulseConstraintSolver::solveGroup( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos  ){
	solveGroupSetup( bodies, infos );
	solveGroupIterate( bodies, infos );
	solveGroupFinish( bodies );
}

void ImpulseConstraintSolver::solveGroupSetup( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos ){
	solverBodyPool.erase( remove_if( solverBodyPool.begin(), solverBodyPool.end(),
		[](SolverBody* element) -> bool {
			delete element;
			return true;
		}
	), solverBodyPool.end() );

	//solverBodyPool.resize( bodies.size() );
	for( uint i = 0; i < bodies.size(); i++ ){
		Entity* body = bodies[i];
		SolverBody* solverBody = getSolverBody( body );

		solverBody->vel += body->force * body->massInv * infoGlobal.m_timeStep;
		solverBody->angVel += body->torque*body->getWorldInertiaInv()*infoGlobal.m_timeStep;
		solverBodyPool.push_back( solverBody );
	}

	for( uint i = 0; i < infos.size(); i++ ){
		convertContact( infos[i] );
	}
}

void ImpulseConstraintSolver::solveGroupIterate( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos ){
	if (infoGlobal.m_splitImpulse){
		for ( int iteration = 0; iteration < infoGlobal.m_numIterations; iteration++) {
			uint numPoolConstraints = contactConstraintPool.size();
			for (uint j = 0 ; j < numPoolConstraints; j++) {
				SolverConstraint& solveManifold = contactConstraintPool[j];
				resolveSplitPenetrationImpulseCacheFriendly( solveManifold );
			}
		}
	}

	for ( int iteration = 0; iteration < infoGlobal.m_numIterations ; iteration++){
		solveSingleIteration(iteration, bodies, infos);
	}
}

void ImpulseConstraintSolver::solveGroupFinish( const vector< Entity* >& bodies ){
	if (infoGlobal.m_solverMode & SOLVER_USE_WARMSTARTING){
		uint numPoolConstraints = contactConstraintPool.size();
		for (uint j = 0; j < numPoolConstraints; j++){
			const SolverConstraint& solveManifold = contactConstraintPool[j];
			ContactInfo* pt = solveManifold.m_originalContactPoint;
			pt->m_appliedImpulse = solveManifold.m_appliedImpulse;

			pt->m_appliedImpulseLateral1 = frictionConstraintPool[solveManifold.m_frictionIndex].m_appliedImpulse;
			if ((infoGlobal.m_solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)){
				pt->m_appliedImpulseLateral2 = frictionConstraintPool[solveManifold.m_frictionIndex+1].m_appliedImpulse;
			}
		}
	}

	for (uint i=0;i<solverBodyPool.size(); i++){
		SolverBody* sb = solverBodyPool[i];
		Entity* body = sb->originalEntity;
		if (body){
			if (infoGlobal.m_splitImpulse) sb->writebackVelocityAndTransform(infoGlobal.m_timeStep, infoGlobal.m_splitImpulseTurnErp);
			else sb->writebackVelocity();

			body->vel = sb->vel;
			body->angVel = sb->angVel;

			if (infoGlobal.m_splitImpulse){
				body->pos = sb->pos;
				body->rot = sb->rot;
			}
		}
	}

	frictionConstraintPool.clear();
	contactConstraintPool.clear();
	rollingFrictionConstraintPool.clear();
}

}
} /* namespace OGLPool */
