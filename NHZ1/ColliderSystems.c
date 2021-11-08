#include "ColliderSystems.h";


bool Collider_checkForCollision(ComponentLists* components, Collider* dynamicC, Collider* staticC, Vec2* contactPoint, Vec2* contactNormal, double* tHitNear, double deltaT) {
	// return false if the dynamic and target colliders are the same
	if (dynamicC->ENTITY_ID == staticC->ENTITY_ID)
		return false;
	
	// return if the dynamic collider's velocity is zero
	PhysicsBody* dynamicPhysicsBody = ECS_getPhysicsBodyComponent(components, dynamicC->ENTITY_ID);
	if (dynamicPhysicsBody == NULL) return false;
	if (dynamicPhysicsBody->velocity.x == 0 && dynamicPhysicsBody->velocity.y == 0)
		return false;
	
	// increase the target collider's size by the dynamic collider's size, so the projected ray can hit the target in every colliding situation
	CollisionBox* targetCollisionBox = ECS_getCollisionBoxComponent(components, staticC->ENTITY_ID);
	CollisionBox* dynamicCollisionBox = ECS_getCollisionBoxComponent(components, dynamicC->ENTITY_ID);
	Vec2_increaseByVec(&targetCollisionBox->size, dynamicCollisionBox->size);
	Position* targetPosition = ECS_getPositionComponent(components, staticC->ENTITY_ID);
	Position* dynamicPosition = ECS_getPositionComponent(components, dynamicC->ENTITY_ID);
	Vec2_increaseByVec(&targetPosition->value, Vec2_imul(dynamicCollisionBox->size, -(1.0/2.0)));

	// check if ray hits the target collider
	Ray2 ray = { 
		.origin = (Vec2) { dynamicPosition->value.x + dynamicCollisionBox->size.x / 2, dynamicPosition->value.y + dynamicCollisionBox->size.y / 2 },
		.destination = Vec2_imul(dynamicPhysicsBody->velocity, deltaT)
	};
	if (CollisionBox_checkIfRayOverlapps(components, targetCollisionBox, ray, contactPoint, contactNormal, tHitNear)) {
		if (*tHitNear <= 1.0) {
			// set the target collider's size to its original value
			Vec2_increaseByVec(&targetCollisionBox->size, Vec2_imul(dynamicCollisionBox->size, -1));
			Vec2_increaseByVec(&targetPosition->value, Vec2_imul(dynamicCollisionBox->size, (1.0 / 2.0)));
			
			return true;
		}
	}

	// set the target collider's size to its original value
	Vec2_increaseByVec(&targetCollisionBox->size, Vec2_imul(dynamicCollisionBox->size, -1));
	Vec2_increaseByVec(&targetPosition->value, Vec2_imul(dynamicCollisionBox->size, (1.0 / 2.0)));
	return false;
}