#include "ColliderSystems.h";


bool Collider_checkForCollision(Layout* currentLayout, Collider* dynamicC, Collider* staticC, Vec2* contactPoint, Vec2* contactNormal, double* tHitNear, double deltaT) {
	// return false if the dynamic and target colliders are the same
	if (dynamicC->ENTITY_ID == staticC->ENTITY_ID)
		return false;

	// get components of the dynamic and static colliders
	void** dynamic = ECS_getEntity(*currentLayout, dynamicC->ENTITY_ID);
	void** target = ECS_getEntity(*currentLayout, staticC->ENTITY_ID);
	
	// return if the dynamic collider's velocity is zero
	PhysicsBody* dynamicPhysicsBody = (PhysicsBody*)dynamic[PHYSICS_BODY];
	if (dynamicPhysicsBody == NULL) return false;
	if (dynamicPhysicsBody->velocity.x == 0 && dynamicPhysicsBody->velocity.y == 0) {
		free(dynamic);
		free(target);
		return false;
	}
	
	// increase the target collider's size by the dynamic collider's size, so the projected ray can hit the target in every colliding situation
	CollisionBox* targetCollisionBox = (CollisionBox*)target[COLLISION_BOX];
	CollisionBox* dynamicCollisionBox = (CollisionBox*)dynamic[COLLISION_BOX];
	Vec2_increaseByVec(&targetCollisionBox->size, dynamicCollisionBox->size);
	Position* targetPosition = (Position*)target[POSITION];
	Position* dynamicPosition = (Position*)dynamic[POSITION];
	Vec2_increaseByVec(&targetPosition->value, Vec2_imul(dynamicCollisionBox->size, -(1.0/2.0)));

	// check if ray hits the target collider
	Ray2 ray = { 
		.origin = (Vec2) { dynamicPosition->value.x + dynamicCollisionBox->size.x / 2, dynamicPosition->value.y + dynamicCollisionBox->size.y / 2 },
		.destination = Vec2_imul(dynamicPhysicsBody->velocity, deltaT)
	};
	if (CollisionBox_checkIfRayOverlapps(currentLayout, targetCollisionBox, ray, contactPoint, contactNormal, tHitNear)) {
		if (*tHitNear <= 1.0) {
			// set the target collider's size to its original value
			Vec2_increaseByVec(&targetCollisionBox->size, Vec2_imul(dynamicCollisionBox->size, -1));
			Vec2_increaseByVec(&targetPosition->value, Vec2_imul(dynamicCollisionBox->size, (1.0 / 2.0)));
			
			free(dynamic);
			free(target);
			return true;
		}
	}

	// set the target collider's size to its original value
	Vec2_increaseByVec(&targetCollisionBox->size, Vec2_imul(dynamicCollisionBox->size, -1));
	Vec2_increaseByVec(&targetPosition->value, Vec2_imul(dynamicCollisionBox->size, (1.0 / 2.0)));

	ECS_freeEntity(dynamic);
	ECS_freeEntity(target);
	return false;
}