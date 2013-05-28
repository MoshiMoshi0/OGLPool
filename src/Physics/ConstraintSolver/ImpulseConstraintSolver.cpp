/*
 * ImpulseConstraintSolver.cpp
 *
 *  Created on: 14-05-2013
 *      Author: _CORE7
 */

#include "ImpulseConstraintSolver.h"
#include <Util/MathUtil.h>
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

void ImpulseConstraintSolver::setupContactConstraint( SolverConstraint& constraint, SolverBody* sb0, SolverBody* sb1, const vec3& r0, const vec3& r1, const vec3& n, ManifoldPoint* info, float& relaxation, vec3& vel, float& rel_vel ){
	RigidBody* rb0 = sb0->originalBody;
	RigidBody* rb1 = sb1->originalBody;

	relaxation = 1.0f;

	{
		vec3 torqueAxis = cross( r0, n );
		if( rb0 ) constraint.angularComponent0 = rb0->getWorldInertiaInv() * torqueAxis;
		constraint.r0CrossN = torqueAxis;
	}
	{
		vec3 torqueAxis = cross( r1, n );
		if( rb1 ) constraint.angularComponent1 = rb1->getWorldInertiaInv() * -torqueAxis;
		constraint.r1CrossN = -torqueAxis;
	}

	float denom0 = 0.0f;
	float denom1 = 0.0f;
	if (rb0){
		vec3 vec = cross(constraint.angularComponent0, r0);
		denom0 = rb0->massInv + dot(n, vec);
	}
	if (rb1){
		vec3 vec = cross(-constraint.angularComponent1, r1);
		denom1 = rb1->massInv + dot(n, vec);
	}

	float denom = relaxation/(denom0+denom1);
	constraint.effectiveMass = denom;
	constraint.contactNormal = n;
	constraint.appliedPushImpulse = 0.f;

	float restitution = 0.0f;
	float penetration = info->depth + infoGlobal.linearSlop;

	vec3 vel1,vel2;

	if( rb0 ) vel1 = rb0->linVel + cross(rb0->angVel, r0);
	if( rb1 ) vel2 = rb1->linVel + cross(rb1->angVel, r1);

	vel = vel1 - vel2;
	rel_vel = dot(n, vel);

	constraint.friction = info->combinedFriction;

	restitution = info->combinedRestitution * -rel_vel;
	if (restitution <= 0.0f){
		restitution = 0.0f;
	}

	if (infoGlobal.solverMode & SOLVER_USE_WARMSTARTING){
		constraint.appliedImpulse = info->appliedImpulse * infoGlobal.warmstartingFactor;

		if( rb0 ) sb0->internalApplyImpulse(constraint.contactNormal * sb0->massInv, constraint.angularComponent0, constraint.appliedImpulse);
		if( rb1 ) sb1->internalApplyImpulse(constraint.contactNormal * sb1->massInv, -constraint.angularComponent1, -constraint.appliedImpulse);
	} else {
		constraint.appliedImpulse = 0.0f;
	}

	{
		float vel1Dotn = 0.0f;
		float vel2Dotn = 0.0f;
		if( rb0 ) vel1Dotn = dot(constraint.contactNormal, sb0->linVel) + dot(constraint.r0CrossN, sb0->angVel);
		if( rb1 ) vel2Dotn = -dot(constraint.contactNormal, sb1->linVel) + dot(constraint.r1CrossN, sb1->angVel);

		float rel_vel = vel1Dotn + vel2Dotn;

		float positionalError = 0.0f;
		float velocityError = restitution - rel_vel;// * damping;

		float erp = infoGlobal.erp2;
		if (!infoGlobal.splitImpulse || (penetration > infoGlobal.splitImpulsePenetrationThreshold)){
			erp = infoGlobal.erp;
		}

		if (penetration > 0){
			positionalError = 0;

			velocityError -= penetration / infoGlobal.timeStep;
		} else {
			positionalError = -penetration * erp / infoGlobal.timeStep;
		}

		float penetrationImpulse = positionalError * constraint.effectiveMass;
		float velocityImpulse = velocityError * constraint.effectiveMass;

		if (!infoGlobal.splitImpulse || (penetration > infoGlobal.splitImpulsePenetrationThreshold)){
			constraint.rhs = penetrationImpulse+velocityImpulse;
			constraint.rhsPenetration = 0.0f;
		} else{
			constraint.rhs = velocityImpulse;
			constraint.rhsPenetration = penetrationImpulse;
		}

		constraint.cfm = 0.0f;
		constraint.lowerLimit = 0.0f;
		constraint.upperLimit = 1e10f;
	}

	contactConstraintPool.push_back( constraint );
}

void ImpulseConstraintSolver::addFrictionConstraint( SolverBody* sb0, SolverBody* sb1, const vec3& r0, const vec3& r1, const vec3& n, ManifoldPoint* info, uint frictionIndex, float& relaxation ){
	SolverConstraint constraint(sb0, sb1);

	RigidBody* rb0 = sb0->originalBody;
	RigidBody* rb1 = sb1->originalBody;

	constraint.friction = info->combinedFriction;
	constraint.frictionIndex = frictionIndex;
	constraint.originalPoint = 0;

	constraint.appliedImpulse = 0.0f;
	constraint.appliedPushImpulse = 0.0f;
	constraint.contactNormal = n;

	{
		vec3 torqueAxis = cross(r0, constraint.contactNormal);
		if( rb0 ) constraint.angularComponent0 = rb0->getWorldInertiaInv() * torqueAxis;
		constraint.r0CrossN = torqueAxis;
	}
	{
		vec3 torqueAxis = cross(r1, -constraint.contactNormal);
		if( rb1 ) constraint.angularComponent1 = rb1->getWorldInertiaInv() * torqueAxis;
		constraint.r1CrossN = torqueAxis;
	}

	float denom0 = 0.0f;
	float denom1 = 0.0f;
	if (rb0) {
		vec3 vec = cross(constraint.angularComponent0, r0);
		denom0 = rb0->massInv + dot(n, vec);
	}
	if (rb1) {
		vec3 vec = cross(-constraint.angularComponent1, r1);
		denom1 = rb1->massInv + dot(n, vec);
	}
	float denom = relaxation/(denom0 + denom1);
	constraint.effectiveMass = denom;

	float vel1Dotn = 0.0f;
	float vel2Dotn = 0.0f;
	if( rb0 ) vel1Dotn = dot(constraint.contactNormal, sb0->linVel)+ dot(constraint.r0CrossN, sb0->angVel);
	if( rb1 ) vel2Dotn = -dot(constraint.contactNormal, sb1->linVel)+ dot(constraint.r1CrossN, sb1->angVel);

	float rel_vel = vel1Dotn + vel2Dotn;

	float velocityError = 0.0f - rel_vel;
	float velocityImpulse = velocityError * constraint.effectiveMass;
	constraint.rhs = velocityImpulse;
	constraint.cfm = 0.0f;
	constraint.lowerLimit = 0;
	constraint.upperLimit = 1e10f;

	frictionConstraintPool.push_back( constraint );
}

void ImpulseConstraintSolver::addRollingFrictionConstraint( SolverBody* sb0, SolverBody* sb1, const vec3& r0, const vec3& r1, const vec3& n, ManifoldPoint* info, uint frictionIndex, float& relaxation ){
	SolverConstraint constraint(sb0, sb1);

	RigidBody* rb0 = sb0->originalBody;
	RigidBody* rb1 = sb1->originalBody;

	constraint.friction = info->combinedRollingFriction;
	constraint.originalPoint = 0;
	constraint.frictionIndex = frictionIndex;

	constraint.appliedImpulse = 0.0f;
	constraint.appliedPushImpulse = 0.0f;
	constraint.contactNormal = vec3();

	{
		vec3 torqueAxis = -n;
		constraint.r0CrossN = torqueAxis;
		if( rb0 ) constraint.angularComponent0 = rb0->getWorldInertiaInv() * torqueAxis;
	}
	{
		vec3 torqueAxis = n;
		constraint.r1CrossN = torqueAxis;
		if( rb1 ) constraint.angularComponent1 = rb1->getWorldInertiaInv() * torqueAxis;
	}

	vec3 iMJaA;
	vec3 iMJaB;
	if( rb0 ) iMJaA = rb0->getWorldInertiaInv() * constraint.r0CrossN;
	if( rb1 ) iMJaB = rb1->getWorldInertiaInv() * constraint.r1CrossN;
	float sum = 0;
	sum += dot(iMJaA, constraint.r0CrossN);
	sum += dot(iMJaB, constraint.r1CrossN);
	constraint.effectiveMass = 1.0f/sum;

	float vel1Dotn = 0.0f;
	float vel2Dotn = 0.0f;
	if( rb0 ) vel1Dotn = dot(constraint.contactNormal, sb0->linVel) + dot(constraint.r0CrossN, sb0->angVel);
	if( rb1 ) vel2Dotn = -dot(constraint.contactNormal, sb1->linVel)+ dot(constraint.r1CrossN, sb1->angVel);

	float rel_vel = vel1Dotn+vel2Dotn;

	float velocityError = 0.0f - rel_vel;
	float velocityImpulse = velocityError * constraint.effectiveMass;
	constraint.rhs = velocityImpulse;
	constraint.cfm = 0.0f;
	constraint.lowerLimit = 0;
	constraint.upperLimit = 1e10f;

	rollingFrictionConstraintPool.push_back( constraint );
}

SolverBody* ImpulseConstraintSolver::getSolverBody( RigidBody* e ){
	if( e && e->mass > 0 ){
		for( auto& sb : solverBodyPool ){
			if( sb->originalBody == e ){
				return sb;
			}
		}
	}

	return solverBodyPool[0];
}

SolverBody* ImpulseConstraintSolver::initSolverBody( RigidBody* e ){
	SolverBody* solverBody = 0;
	if( e ){
		solverBody = new SolverBody( e );
	}else{
		solverBody = new SolverBody();

		solverBody->originalBody = 0;
		solverBody->massInv = 0;
		solverBody->linVel = vec3();
		solverBody->angVel = vec3();
		solverBody->pos = vec3();
		solverBody->rot = quat();
	}

	solverBody->deltaLinearVelocity = vec3();
	solverBody->deltaAngularVelocity = vec3();
	solverBody->pushVelocity = vec3();
	solverBody->turnVelocity = vec3();
	return solverBody;
}

void ImpulseConstraintSolver::setFrictionConstraintImpulse( SolverConstraint& constraint, SolverBody* sb0, SolverBody* sb1, ManifoldPoint* info ){
	RigidBody* rb0 = sb0->originalBody;
	RigidBody* rb1 = sb1->originalBody;

	SolverConstraint& frictionConstraint1 = frictionConstraintPool[constraint.frictionIndex];
	if (infoGlobal.solverMode & SOLVER_USE_WARMSTARTING) {
		frictionConstraint1.appliedImpulse = info->appliedImpulseLateral1 * infoGlobal.warmstartingFactor;
		if (rb0) sb0->internalApplyImpulse(frictionConstraint1.contactNormal * rb0->massInv, frictionConstraint1.angularComponent0, frictionConstraint1.appliedImpulse);
		if (rb1) sb1->internalApplyImpulse(frictionConstraint1.contactNormal * rb1->massInv, -frictionConstraint1.angularComponent1, -frictionConstraint1.appliedImpulse);
	} else {
		frictionConstraint1.appliedImpulse = 0.f;
	}

	if ((infoGlobal.solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)){
		SolverConstraint& frictionConstraint2 = frictionConstraintPool[constraint.frictionIndex+1];
		if (infoGlobal.solverMode & SOLVER_USE_WARMSTARTING) {
			frictionConstraint2.appliedImpulse = info->appliedImpulseLateral2  * infoGlobal.warmstartingFactor;
			if (rb0) sb0->internalApplyImpulse(frictionConstraint2.contactNormal * rb0->massInv, frictionConstraint2.angularComponent0, frictionConstraint2.appliedImpulse);
			if (rb1) sb1->internalApplyImpulse(frictionConstraint2.contactNormal * rb1->massInv, -frictionConstraint2.angularComponent1, -frictionConstraint2.appliedImpulse);
		} else {
			frictionConstraint2.appliedImpulse = 0.f;
		}
	}
}

void ImpulseConstraintSolver::convertContact( ContactManifold* manifold ){
	RigidBody* e0 = manifold->e0;
	RigidBody* e1 = manifold->e1;
	SolverBody* sb0 = getSolverBody( e0 );
	SolverBody* sb1 = getSolverBody( e1 );

	assert( sb0 != sb1 );
	int rollingFrictionCount = 1;
	for( int i = 0; i < manifold->pointsAdded; i++ ){
		ManifoldPoint* info = manifold->points[ i ];
		assert( info );
		vec3 n = info->normal;
		vec3 r0 = info->point0 - sb0->pos;
		vec3 r1 = info->point1 - sb1->pos;

		float relaxation = 0.0f;
		float rel_vel = 0.0f;
		vec3 vel;

		uint frictionIndex = contactConstraintPool.size();
		SolverConstraint constraint( sb0, sb1 );
		constraint.originalPoint = info;
		constraint.frictionIndex = frictionConstraintPool.size();
		setupContactConstraint( constraint, sb0, sb1, r0, r1, n, info, relaxation, vel, rel_vel );
		if( rollingFrictionCount-- > 0 ){
			vec3 da = sb1->angVel - sb0->angVel;
			if( info->combinedRollingFriction > 0.f){
				if( length( da ) > infoGlobal.singleAxisRollingFrictionThreshold ){
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
		}

		if (!(infoGlobal.solverMode & SOLVER_ENABLE_FRICTION_DIRECTION_CACHING) || !info->lateralFrictionInitialized ){
			info->lateralFrictionDir1 = vel - n * rel_vel;
			float lat_rel_vel = dot( info->lateralFrictionDir1, info->lateralFrictionDir1 );
			if( !(infoGlobal.solverMode & SOLVER_DISABLE_VELOCITY_DEPENDENT_FRICTION_DIRECTION) && lat_rel_vel > epsilon<float>() ){
				info->lateralFrictionDir1 *= 1.f/sqrt(lat_rel_vel);
				if((infoGlobal.solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)) {
					info->lateralFrictionDir2 = normalize( cross( info->lateralFrictionDir1, n ) );
					addFrictionConstraint( sb0, sb1, r0, r1, info->lateralFrictionDir2, info, frictionIndex, relaxation );
				}
				addFrictionConstraint( sb0, sb1, r0, r1, info->lateralFrictionDir1, info, frictionIndex, relaxation );
			}else{
				planeSpace(n,info->lateralFrictionDir1,info->lateralFrictionDir2);

				if ((infoGlobal.solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)){
					addFrictionConstraint( sb0, sb1, r0, r1, info->lateralFrictionDir2, info, frictionIndex, relaxation );
				}

				addFrictionConstraint( sb0, sb1, r0, r1, info->lateralFrictionDir1, info, frictionIndex, relaxation );
				if ((infoGlobal.solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS) && (infoGlobal.solverMode & SOLVER_DISABLE_VELOCITY_DEPENDENT_FRICTION_DIRECTION)) {
					info->lateralFrictionInitialized = true;
				}
			}
		} else {
			addFrictionConstraint( sb0, sb1, r0, r1, info->lateralFrictionDir1, info, frictionIndex, relaxation );
			if ((infoGlobal.solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS))
				addFrictionConstraint( sb0, sb1, r0, r1, info->lateralFrictionDir2, info, frictionIndex, relaxation );

			setFrictionConstraintImpulse( constraint, sb0, sb1, info );
		}
	}
}

void ImpulseConstraintSolver::solveSingleIteration( int iteration, const vector< RigidBody* >& bodies, const vector< ContactManifold* >& infos ){
	if (iteration >= infoGlobal.iterations) assert(0);

	if (infoGlobal.solverMode & SOLVER_RANDMIZE_ORDER){
		random_shuffle( contactConstraintPool.begin(), contactConstraintPool.end() );
		random_shuffle( frictionConstraintPool.begin(), frictionConstraintPool.end() );
	}

	uint numPoolConstraints = contactConstraintPool.size();
	for (uint j = 0; j < numPoolConstraints; j++){
		SolverConstraint& solveManifold = contactConstraintPool[j];
		resolveSingleConstraintRowLowerLimit(solveManifold);
	}

	uint numFrictionPoolConstraints = frictionConstraintPool.size();
	for (uint j = 0; j < numFrictionPoolConstraints; j++){
		SolverConstraint& solveManifold = frictionConstraintPool[j];
		const SolverConstraint& contactConstraint = contactConstraintPool[solveManifold.frictionIndex];
		float totalImpulse = contactConstraint.appliedImpulse;

		if (totalImpulse > 0) {
			solveManifold.lowerLimit = -(solveManifold.friction * totalImpulse);
			solveManifold.upperLimit = solveManifold.friction * totalImpulse;

			resolveSingleConstraintRowGeneric(solveManifold);
		}
	}

	uint numRollingFrictionPoolConstraints = rollingFrictionConstraintPool.size();
	for (uint j = 0; j < numRollingFrictionPoolConstraints; j++){
		SolverConstraint& rollingFrictionConstraint = rollingFrictionConstraintPool[j];
		const SolverConstraint& contactConstraint = contactConstraintPool[rollingFrictionConstraint.frictionIndex];
		float totalImpulse = contactConstraint.appliedImpulse;

		if (totalImpulse > 0){
			float rollingFrictionMagnitude = rollingFrictionConstraint.friction * totalImpulse;
			if (rollingFrictionMagnitude > rollingFrictionConstraint.friction)
				rollingFrictionMagnitude = rollingFrictionConstraint.friction;

			rollingFrictionConstraint.lowerLimit = -rollingFrictionMagnitude;
			rollingFrictionConstraint.upperLimit = rollingFrictionMagnitude;

			resolveSingleConstraintRowGeneric(rollingFrictionConstraint);
		}
	}
}

void ImpulseConstraintSolver::resolveSingleConstraintRowLowerLimit( SolverConstraint& constraint ){
	SolverBody* rb0 = constraint.sb0;
	SolverBody* rb1 = constraint.sb1;

	float deltaImpulse = constraint.rhs - constraint.appliedImpulse * constraint.cfm;
	float deltaVel1Dotn = dot(constraint.contactNormal, rb0->deltaLinearVelocity) + dot(constraint.r0CrossN, rb0->deltaAngularVelocity);
	float deltaVel2Dotn = -dot(constraint.contactNormal, rb1->deltaLinearVelocity) + dot(constraint.r1CrossN, rb1->deltaAngularVelocity);

	deltaImpulse -= deltaVel1Dotn * constraint.effectiveMass;
	deltaImpulse -= deltaVel2Dotn * constraint.effectiveMass;
	const float sum = constraint.appliedImpulse + deltaImpulse;
	if (sum < constraint.lowerLimit){
		deltaImpulse = constraint.lowerLimit - constraint.appliedImpulse;
		constraint.appliedImpulse = constraint.lowerLimit;
	}else{
		constraint.appliedImpulse = sum;
	}

	rb0->internalApplyImpulse(constraint.contactNormal * rb0->massInv, constraint.angularComponent0, deltaImpulse);
	rb1->internalApplyImpulse(-constraint.contactNormal * rb1->massInv, constraint.angularComponent1, deltaImpulse);
}

void ImpulseConstraintSolver::resolveSingleConstraintRowGeneric( SolverConstraint& constraint ){
	SolverBody* rb0 = constraint.sb0;
	SolverBody* rb1 = constraint.sb1;

	float deltaImpulse = constraint.rhs - constraint.appliedImpulse * constraint.cfm;
	float deltaVel1Dotn =  dot(constraint.contactNormal, rb0->deltaLinearVelocity) + dot(constraint.r0CrossN, rb0->deltaAngularVelocity);
	float deltaVel2Dotn = -dot(constraint.contactNormal, rb1->deltaLinearVelocity) + dot(constraint.r1CrossN, rb1->deltaAngularVelocity);

	deltaImpulse -= deltaVel1Dotn * constraint.effectiveMass;
	deltaImpulse -= deltaVel2Dotn * constraint.effectiveMass;

	float sum = constraint.appliedImpulse + deltaImpulse;
	if (sum < constraint.lowerLimit) {
		deltaImpulse = constraint.lowerLimit - constraint.appliedImpulse;
		constraint.appliedImpulse = constraint.lowerLimit;
	} else if (sum > constraint.upperLimit)  {
		deltaImpulse = constraint.upperLimit - constraint.appliedImpulse;
		constraint.appliedImpulse = constraint.upperLimit;
	} else {
		constraint.appliedImpulse = sum;
	}

	rb0->internalApplyImpulse(constraint.contactNormal * rb0->massInv, constraint.angularComponent0, deltaImpulse);
	rb1->internalApplyImpulse(-constraint.contactNormal * rb1->massInv, constraint.angularComponent1, deltaImpulse);
}

void ImpulseConstraintSolver::resolveSplitPenetrationImpulseCacheFriendly( SolverConstraint& constraint ){
	SolverBody* rb0 = constraint.sb0;
	SolverBody* rb1 = constraint.sb1;

	if (constraint.rhsPenetration) {
		float deltaImpulse = constraint.rhsPenetration - constraint.appliedPushImpulse * constraint.cfm;
		float deltaVel1Dotn =  dot(constraint.contactNormal, rb0->pushVelocity) + dot( constraint.r0CrossN, rb0->turnVelocity );
		float deltaVel2Dotn = -dot(constraint.contactNormal, rb1->pushVelocity) + dot( constraint.r1CrossN, rb1->turnVelocity );

		deltaImpulse -= deltaVel1Dotn * constraint.effectiveMass;
		deltaImpulse -= deltaVel2Dotn * constraint.effectiveMass;
		float sum = constraint.appliedPushImpulse + deltaImpulse;
		if (sum < constraint.lowerLimit) {
			deltaImpulse = constraint.lowerLimit - constraint.appliedPushImpulse;
			constraint.appliedPushImpulse = constraint.lowerLimit;
		} else {
			constraint.appliedPushImpulse = sum;
		}

		rb0->internalApplyPushImpulse(constraint.contactNormal * rb0->massInv, constraint.angularComponent0, deltaImpulse);
		rb1->internalApplyPushImpulse(-constraint.contactNormal * rb1->massInv, constraint.angularComponent1, deltaImpulse);
	}
}

void ImpulseConstraintSolver::solveGroup( const vector< RigidBody* >& bodies, const vector< ContactManifold* >& infos, float dt ){
	infoGlobal.timeStep = dt;

	solveGroupSetup( bodies, infos );
	solveGroupIterate( bodies, infos );
	solveGroupFinish( bodies );
}

void ImpulseConstraintSolver::solveGroupSetup( const vector< RigidBody* >& bodies, const vector< ContactManifold* >& infos ){
	solverBodyPool.erase( remove_if( solverBodyPool.begin(), solverBodyPool.end(),
		[](SolverBody* element) -> bool {
			delete element;
			return true;
		}
	), solverBodyPool.end() );

	solverBodyPool.reserve( bodies.size() + 1 );
	SolverBody* fixedBody = initSolverBody( 0 );
	solverBodyPool.push_back( fixedBody );

	for( uint i = 0; i < bodies.size(); i++ ){
		RigidBody* body = bodies[i];

		SolverBody* solverBody = getSolverBody( body );
		if( body && body->mass > 0){
			if( solverBody == fixedBody ){
				solverBody = initSolverBody( body );
				solverBodyPool.push_back( solverBody );
			}

			solverBody->linVel += body->force * body->massInv * infoGlobal.timeStep;
			solverBody->angVel += body->torque * body->getWorldInertiaInv() * infoGlobal.timeStep;
		}
	}

	for( uint i = 0; i < infos.size(); i++ ){
		convertContact( infos[i] );
	}
}

void ImpulseConstraintSolver::solveGroupIterate( const vector< RigidBody* >& bodies, const vector< ContactManifold* >& infos ){
	if (infoGlobal.splitImpulse){
		for ( int iteration = 0; iteration < infoGlobal.iterations; iteration++) {
			uint numPoolConstraints = contactConstraintPool.size();
			for (uint j = 0 ; j < numPoolConstraints; j++) {
				resolveSplitPenetrationImpulseCacheFriendly( contactConstraintPool[j] );
			}
		}
	}

	for ( int iteration = 0; iteration < infoGlobal.iterations; iteration++){
		solveSingleIteration(iteration, bodies, infos);
	}
}

void ImpulseConstraintSolver::solveGroupFinish( const vector< RigidBody* >& bodies ){
	if (infoGlobal.solverMode & SOLVER_USE_WARMSTARTING){
		uint numPoolConstraints = contactConstraintPool.size();
		for (uint i = 0; i < numPoolConstraints; i++){
			const SolverConstraint& solveManifold = contactConstraintPool[i];
			ManifoldPoint* info = solveManifold.originalPoint;
			info->appliedImpulse = solveManifold.appliedImpulse;

			info->appliedImpulseLateral1 = frictionConstraintPool[solveManifold.frictionIndex].appliedImpulse;
			if ((infoGlobal.solverMode & SOLVER_USE_2_FRICTION_DIRECTIONS)){
				info->appliedImpulseLateral2 = frictionConstraintPool[solveManifold.frictionIndex+1].appliedImpulse;
			}
		}
	}

	for (uint i = 0; i < solverBodyPool.size(); i++){
		SolverBody* sb = solverBodyPool[i];
		RigidBody* rb = sb->originalBody;
		if ( rb ){
			if (infoGlobal.splitImpulse) sb->writebackVelocityAndTransform(infoGlobal.timeStep, infoGlobal.splitImpulseTurnErp);
			else sb->writebackVelocity();

			rb->linVel = sb->linVel;
			rb->angVel = sb->angVel;

			if (infoGlobal.splitImpulse){
				rb->pos = sb->pos;
				rb->rot = sb->rot;
			}
		}
	}

	frictionConstraintPool.clear();
	contactConstraintPool.clear();
	rollingFrictionConstraintPool.clear();
}

}
} /* namespace OGLPool */
