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

ImpulseConstraintSolver::~ImpulseConstraintSolver(){}

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

void ImpulseConstraintSolver::addContactConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, float& relaxation, vec3& vel, float& rel_vel ){
	SolverConstraint constraint(sb0, sb1);

	/*vec3 a0 = cross( sb0->angVel, r0 );
	vec3 a1 = cross( sb1->angVel, r1 );
	vec3 v0 = sb0->getVel() + a0;
	vec3 v1 = sb1->getVel() + a1;
	vec3 dv = v0 - v1;

	float denom = sb0->massInv + sb1->massInv + dot( sb0->getWorldInertiaInv() * cross(cross(r0,n),r0) + sb1->getWorldInertiaInv() * cross(cross(r1,n),r1), n );

	float penetration = glm::max( info->depth - 0.01f, 0.f );

	float num = -(1+e) * dot(dv,n);
	if( penetration > 0.04 ){
		float posError = (penetration - 0.04) / info->deltaTime;
		num += posError * 0.8f;
	}

	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;
	if( jr > 1e10f ) jr = 1e10f;*/

	const vec3& pos1 = info->point0;
	const vec3& pos2 = info->point1;

	Entity* rb0 = sb0->originalEntity;
	Entity* rb1 = sb1->originalEntity;

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

			float denom = 1.0f/(denom0+denom1);
			constraint.m_jacDiagABInv = denom;


		constraint.m_contactNormal = n;
		constraint.m_relpos1CrossNormal = torqueAxis0;
		constraint.m_relpos2CrossNormal = -torqueAxis1;

		float restitution = 0.f;
		float penetration = info->depth+infoGlobal.m_linearSlop;

		{
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
		float	velocityError = restitution - rel_vel;// * damping;


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
		constraint.m_cfm = 0.f;
		constraint.m_lowerLimit = 0;
		constraint.m_upperLimit = 1e10f;
	}


	contactConstraintPool.push_back( constraint );
}

void ImpulseConstraintSolver::addFrictionConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, float& relaxation ){
	SolverConstraint constraint(sb0, sb1);

	/*vec3 a0 = cross( sb0->angVel, r0 );
	vec3 a1 = cross( sb1->angVel, r1 );
	vec3 v0 = sb0->getVel() + a0;
	vec3 v1 = sb1->getVel() + a1;
	vec3 dv = v1 - v0;

	float num = e * dot( dv, n );
	float denom = sb0->massInv + sb1->massInv + dot( sb0->getWorldInertiaInv() * cross(cross(r0,n),r0) + sb1->getWorldInertiaInv() * cross(cross(r1,n),r1), n );
	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;
	if( jr > 1e10f ) jr = 1e10f;*/

	vec3 normalAxis(0,0,0);

	constraint.m_contactNormal = normalAxis;

	Entity* body0 = sb0->originalEntity;
	Entity* body1 = sb1->originalEntity;

		constraint.m_friction = info->m_combinedFriction;
		//constraint.m_originalContactPoint = 0;

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
		denom0 = body0->massInv + dot(normalAxis,vec);
	}
	if (body1) {
		vec = cross(-constraint.m_angularComponentB,r1);
		denom1 = body1->massInv + dot(normalAxis,vec);
	}
	float denom = relaxation/(denom0+denom1);
	constraint.m_jacDiagABInv = denom;

	float rel_vel;
	float vel1Dotn = dot(constraint.m_contactNormal,sb0->vel)+ dot(constraint.m_relpos1CrossNormal,sb0->angVel);
	float vel2Dotn = -dot(constraint.m_contactNormal, sb1->vel)+ dot(constraint.m_relpos2CrossNormal,sb1->angVel);

	rel_vel = vel1Dotn+vel2Dotn;

	float velocityError =  0.0f - rel_vel;
	float	velocityImpulse = velocityError * float(constraint.m_jacDiagABInv);
	constraint.m_rhs = velocityImpulse;
	constraint.m_cfm = 1.0f;
	constraint.m_lowerLimit = 0;
	constraint.m_upperLimit = 1e10f;

	frictionConstraintPool.push_back( constraint );
}

void ImpulseConstraintSolver::addRollingFrictionConstraint( SolverBody* sb0, SolverBody* sb1, vec3 r0, vec3 r1, vec3 n, ContactInfo* info, float& relaxation ){
	SolverConstraint constraint(sb0, sb1);

	/*vec3 a0 = cross( sb0->angVel, r0 );
	vec3 a1 = cross( sb1->angVel, r1 );

	float num = e * (dot(n,a1) + dot(n,a0));
	float denom = dot( sb0->getWorldInertiaInv() * n + sb1->getWorldInertiaInv() * n, n );
	float jr = num / denom;

	if( jr < 0 ) jr = 0.0f;
	if( jr > 1e10f ) jr = 1e10f;*/

	vec3 normalAxis(0,0,0);

	constraint.m_contactNormal = normalAxis;

	Entity* body0 = sb0->originalEntity;
	Entity* body1 = sb1->originalEntity;

	constraint.m_friction = info->m_combinedRollingFriction;
	//constraint.m_originalContactPoint = 0;

	constraint.m_appliedImpulse = 0.f;
	constraint.m_appliedPushImpulse = 0.f;

	constraint.m_relpos1CrossNormal = -n;
	constraint.m_angularComponentA = body0->getWorldInertiaInv()*(-n);

	constraint.m_relpos2CrossNormal = n;
	constraint.m_angularComponentB = body1->getWorldInertiaInv()*n;

	vec3 iMJaA = body0->getWorldInertiaInv()*constraint.m_relpos1CrossNormal;
	vec3 iMJaB = body1->getWorldInertiaInv()*constraint.m_relpos2CrossNormal;
	float sum = 0;
	sum += dot(iMJaA, constraint.m_relpos1CrossNormal);
	sum += dot(iMJaB, constraint.m_relpos2CrossNormal);
	constraint.m_jacDiagABInv = float(1.)/sum;

	float vel1Dotn = dot(constraint.m_contactNormal, sb0->vel) + dot(constraint.m_relpos1CrossNormal,sb0->angVel);
	float vel2Dotn = -dot(constraint.m_contactNormal, sb1->vel)+ dot(constraint.m_relpos2CrossNormal, sb1->angVel);

	float rel_vel = vel1Dotn+vel2Dotn;

	float velocityError =  0.0f - rel_vel;
	float	velocityImpulse = velocityError * float(constraint.m_jacDiagABInv);
	constraint.m_rhs = velocityImpulse;
	constraint.m_cfm = 1.0f;// cfmSlip
	constraint.m_lowerLimit = 0;
	constraint.m_upperLimit = 1e10f;

	rollingFrictionConstraintPool.push_back( constraint );
}

SolverBody* ImpulseConstraintSolver::getSolverBody( Entity* e ){
	for( auto& sb : solverBodyPool ){
		if( sb->originalEntity == e ){
			return sb;
		}
	}

	assert( 0 );
	return 0;
}

void ImpulseConstraintSolver::convertContact( ContactInfo* info ){
	Entity* e0 = info->e1;
	Entity* e1 = info->e0;
	SolverBody* sb0 = getSolverBody( e0 );
	SolverBody* sb1 = getSolverBody( e1 );

	vec3 n = info->normal;
	vec3 r0 = info->point0 - sb0->pos;
	vec3 r1 = info->point1 - sb1->pos;

	float relaxation;
	float rel_vel;
	vec3 vel;
	addContactConstraint( sb0, sb1, r0, r1, n, info, relaxation, vel, rel_vel );

	vec3 da = sb1->angVel - sb0->angVel;
	if( length( da ) > 1e30f ){
		da = normalize( da );
		if ( length(da) > 0.001f ) addRollingFrictionConstraint( sb0, sb1, r0, r1, da, info, relaxation );
	}else{
		vec3 t0,t1;
		planeSpace( n, t0, t1 );

		addRollingFrictionConstraint( sb0, sb1, r0, r1, n, info, relaxation );
		if ( length( t0 ) > 0.001f ) addRollingFrictionConstraint( sb0, sb1, r0, r1, t0, info, relaxation );
		if ( length( t1 ) > 0.001f ) addRollingFrictionConstraint( sb0, sb1, r0, r1, t1, info, relaxation );
	}

	vec3 t = vel - rel_vel * n;
	if( dot( t,t ) > epsilon<float>() ){
		vec3 t0 = normalize( t );
		vec3 t1 = cross( n, t0 );
		addFrictionConstraint( sb0, sb1, r0, r1, t0, info, relaxation );
		addFrictionConstraint( sb0, sb1, r0, r1, t1, info, relaxation );
	} else {
		vec3 t0,t1;
		planeSpace( n, t0, t1 );

		addFrictionConstraint( sb0, sb1, r0, r1, t0, info, relaxation );
		addFrictionConstraint( sb0, sb1, r0, r1, t1, info, relaxation );
	}
}

void ImpulseConstraintSolver::solveGroup( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos  ){
	solveGroupSetup( bodies, infos );
	solveGroupIterate( bodies, infos );
	solveGroupFinish( bodies, infos );
}

void ImpulseConstraintSolver::solveGroupSetup( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos ){
	contactConstraintPool = vector< SolverConstraint >();
	frictionConstraintPool = vector< SolverConstraint >();
	rollingFrictionConstraintPool = vector< SolverConstraint >();
	solverBodyPool = vector< SolverBody* >( bodies.size() );

	for( int i = 0; i < bodies.size(); i++ ){
		Entity* body = bodies.at( i );

		//if( body->massInv ){
			SolverBody* sb = new SolverBody( bodies[i] );
			solverBodyPool[i] = sb;
		//}else{
		//	solverBodyPool[i] = 0;
		//}
	}

	for( int i = 0; i < infos.size(); i++ ){
		convertContact( infos[i] );
	}
}

void ImpulseConstraintSolver::solveGroupIterate( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos ){
	for ( int iteration = 0 ; iteration < infoGlobal.m_numIterations ; iteration++){
		solveSingleIteration(iteration, bodies, infos);
	}
}

void ImpulseConstraintSolver::solveSingleIteration( int iteration, const vector< Entity* >& bodies, const vector< ContactInfo* >& infos ){
	if (iteration >= infoGlobal.m_numIterations) assert(0);

	/*for (int j = 0; j < numConstraints; j++){
		if (constraints[j]->isEnabled()){
			int bodyAid = getOrInitSolverBody(constraints[j]->getRigidBodyA());
			int bodyBid = getOrInitSolverBody(constraints[j]->getRigidBodyB());
			btSolverBody& bodyA = m_tmpSolverBodyPool[bodyAid];
			btSolverBody& bodyB = m_tmpSolverBodyPool[bodyBid];
			constraints[j]->solveConstraintObsolete(bodyA,bodyB,info->m_timeStep);
		}
	}*/

	int numPoolConstraints = contactConstraintPool.size();
	for (int j=0;j<numPoolConstraints;j++){
		SolverConstraint& solveManifold = contactConstraintPool[j];
		resolveSingleConstraintRowLowerLimit(solveManifold);
	}

	int numFrictionPoolConstraints = frictionConstraintPool.size();
	for( int i = 0; i < numFrictionPoolConstraints; i++ ){
		SolverConstraint& solveManifold = frictionConstraintPool[i];
	}

	/*int numFrictionPoolConstraints = m_tmpSolverContactFrictionConstraintPool.size();
	for (int j=0;j<numFrictionPoolConstraints;j++){
		SolverConstraint& solveManifold = m_tmpSolverContactFrictionConstraintPool[m_orderFrictionConstraintPool[j]];
		float totalImpulse = m_tmpSolverContactConstraintPool[solveManifold.m_frictionIndex].m_appliedImpulse;

		if (totalImpulse>float(0))
		{
			solveManifold.m_lowerLimit = -(solveManifold.m_friction*totalImpulse);
			solveManifold.m_upperLimit = solveManifold.m_friction*totalImpulse;

			resolveSingleConstraintRowGeneric(m_tmpSolverBodyPool[solveManifold.m_solverBodyIdA],m_tmpSolverBodyPool[solveManifold.m_solverBodyIdB],solveManifold);
		}
	}

	int numRollingFrictionPoolConstraints = m_tmpSolverContactRollingFrictionConstraintPool.size();
	for (int j=0;j<numRollingFrictionPoolConstraints;j++){
		SolverConstraint& rollingFrictionConstraint = m_tmpSolverContactRollingFrictionConstraintPool[j];
		float totalImpulse = m_tmpSolverContactConstraintPool[rollingFrictionConstraint.m_frictionIndex].m_appliedImpulse;
		if (totalImpulse>float(0))
		{
			float rollingFrictionMagnitude = rollingFrictionConstraint.m_friction*totalImpulse;
			if (rollingFrictionMagnitude>rollingFrictionConstraint.m_friction)
				rollingFrictionMagnitude = rollingFrictionConstraint.m_friction;

			rollingFrictionConstraint.m_lowerLimit = -rollingFrictionMagnitude;
			rollingFrictionConstraint.m_upperLimit = rollingFrictionMagnitude;

			resolveSingleConstraintRowGeneric(m_tmpSolverBodyPool[rollingFrictionConstraint.m_solverBodyIdA],m_tmpSolverBodyPool[rollingFrictionConstraint.m_solverBodyIdB],rollingFrictionConstraint);
		}
	}*/
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

void ImpulseConstraintSolver::solveGroupFinish( const vector< Entity* >& bodies, const vector< ContactInfo* >& infos ){
	for (int i=0;i<solverBodyPool.size(); i++){
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

			//solverBodyPool[i].m_originalBody->setCompanionId(-1);
		}
	}

	solverBodyPool.erase( remove_if( solverBodyPool.begin(), solverBodyPool.end(),
		[](Entity* element) -> bool {
			delete element;
			return true;
		}
	), solverBodyPool.end() );

	solverBodyPool.clear();
	frictionConstraintPool.clear();
	contactConstraintPool.clear();
	rollingFrictionConstraintPool.clear();
}

}
} /* namespace OGLPool */
