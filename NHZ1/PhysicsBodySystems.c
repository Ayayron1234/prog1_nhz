#include "PhysicsBodySystems.h"

void PhysicsBody_update(Layout* currentLayout, PhysicsBody* physicsBody, double deltaT) {
	void** dynamicComps = ECS_getEntity(*currentLayout, physicsBody->ENTITY_ID);
	Position* position = (Position*)dynamicComps[POSITION];
	
	Vec2 sumAcceleration = Vec2_add(physicsBody->acceleration, Vec2_imul(physicsBody->gravitationalAcceleration, physicsBody->mass));
	physicsBody->velocity = Vec2_add(physicsBody->velocity, Vec2_imul(sumAcceleration, deltaT));

	Collider* dynamicCollider = (Collider*)dynamicComps[COLLIDER];
	if (dynamicCollider) {
		bool didCollide = false;
		for (int i = 0; i < ECS_getNumberOfComponents(COLLIDER, *currentLayout); i++) {
			Collider* staticCollider = &((Collider*)ECS_getComponentList(COLLIDER, *currentLayout))[i];
			if (staticCollider->type == DYNAMIC) continue;
			double tHitNear = 0;
			Vec2 contactPoint;
			Vec2 contactNormal;
			if (Collider_checkForCollision(currentLayout, (Collider*)dynamicComps[COLLIDER],
				ECS_getComponent(COLLIDER, *currentLayout, staticCollider->ENTITY_ID), &contactPoint, &contactNormal, &tHitNear, deltaT)) 
			{
				Vec2_increaseByVec(&physicsBody->velocity,
					Vec2_imul((Vec2) { contactNormal.x* fabs(physicsBody->velocity.x), contactNormal.y* fabs(physicsBody->velocity.y) }, 1 - tHitNear));
			}
		}
	} 
	Position_moveBy(position, Vec2_imul(physicsBody->velocity, deltaT));

	free(dynamicComps);
}
