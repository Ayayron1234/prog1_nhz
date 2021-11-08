#include "PhysicsBodySystems.h"

void PhysicsBody_update(ComponentLists* components, PhysicsBody* physicsBody, double deltaT) {
	Position* position = ECS_getPositionComponent(components, physicsBody->ENTITY_ID);
	
	Vec2 sumAcceleration = Vec2_add(physicsBody->acceleration, Vec2_imul(physicsBody->gravitationalAcceleration, physicsBody->mass));
	physicsBody->velocity = Vec2_add(physicsBody->velocity, Vec2_imul(sumAcceleration, deltaT));

	Collider* dynamicCollider = ECS_getColliderComponent(components, physicsBody->ENTITY_ID);
	if (dynamicCollider) {
		bool didCollide = false;
		for (int i = 0; i < components->total_colliderComponents; i++) {
			if (components->colliderComponents[i].type == DYNAMIC) continue;
			double tHitNear = 0;
			Vec2 contactPoint;
			Vec2 contactNormal;
			if (Collider_checkForCollision(components, ECS_getColliderComponent(components, physicsBody->ENTITY_ID),
				ECS_getColliderComponent(components, components->colliderComponents[i].ENTITY_ID), &contactPoint, &contactNormal, &tHitNear, deltaT)) 
			{
				Vec2_increaseByVec(&physicsBody->velocity,
					Vec2_imul((Vec2) { contactNormal.x* fabs(physicsBody->velocity.x), contactNormal.y* fabs(physicsBody->velocity.y) }, 1 - tHitNear));
			}
		}
	} 
	Position_moveBy(position, Vec2_imul(physicsBody->velocity, deltaT));
}
