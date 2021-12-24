#include "PhysicsBodySystems.h"

void PhysicsBody_update(Layout* currentLayout, PhysicsBody* physicsBody, double deltaT, GameState* gameState) {
	void** dynamicComps = ECS_getEntity(*currentLayout, physicsBody->ENTITY_ID);
	Position* position = (Position*)dynamicComps[POSITION];
	
	Vec2 sumAcceleration = Vec2_add(physicsBody->acceleration, Vec2_imul(physicsBody->gravitationalAcceleration, physicsBody->mass));
	physicsBody->velocity = Vec2_add(physicsBody->velocity, Vec2_imul(sumAcceleration, deltaT));

	MovementController* movementController = dynamicComps[MOVEMENT_CONTROLLER];

	// check if entity is moving faster than it's max movement speed
	if (movementController != NULL) {
		double v = sqrt(pow(physicsBody->velocity.x, 2) + pow(physicsBody->velocity.y, 2));
		if (v > movementController->maxFallSpeed) {
			double vs = movementController->maxFallSpeed / v;
			physicsBody->velocity.x *= vs;
			physicsBody->velocity.y *= vs;
		}
	}

	Collider* dynamicCollider = (Collider*)dynamicComps[COLLIDER];
	if (dynamicCollider) {
		bool didCollide = false;
		if (movementController != NULL)
			movementController->collisionNormal = (Vec2){ 0, 0 };

		// required for performance enhancement
		double velocityLength = sqrt(pow(physicsBody->velocity.x, 2) + pow(physicsBody->velocity.y, 2)) / 10 + 20;
		SDL_Rect A = { .x = position->value.x - velocityLength / 2, .y = position->value.y - velocityLength / 2,
			.w = ((CollisionBox*)dynamicComps[COLLISION_BOX])->size.x + velocityLength, .h = ((CollisionBox*)dynamicComps[COLLISION_BOX])->size.y + velocityLength };

		for (int i = 0; i < ECS_getNumberOfComponents(COLLIDER, *currentLayout); i++) {
			Collider* staticCollider = &((Collider*)ECS_getComponentList(COLLIDER, *currentLayout))[i];

			// check if collision boxes are near to each other for better performance
			Position* secondPosition = ECS_getComponent(POSITION, *currentLayout, staticCollider->ENTITY_ID);
			CollisionBox* secondCollisionBox = ECS_getComponent(COLLISION_BOX, *currentLayout, staticCollider->ENTITY_ID);
			SDL_Rect B = { .x = secondPosition->value.x, .y = secondPosition->value.y,
				.w = secondCollisionBox->size.x, .h = secondCollisionBox->size.y };
			if (!CollisionBox_checkForOverlapp(A, B)) continue;

			double tHitNear = 0;
			Vec2 contactPoint;
			Vec2 contactNormal;
			if (Collider_checkForCollision(currentLayout, (Collider*)dynamicComps[COLLIDER],
				ECS_getComponent(COLLIDER, *currentLayout, staticCollider->ENTITY_ID), &contactPoint, &contactNormal, &tHitNear, deltaT)) 
			{
				// register interaction
				Interactable* interactable = ECS_getComponent(INTERACTABLE, *currentLayout, staticCollider->ENTITY_ID);
				if (NULL != interactable) {
					if (interactable->activeInteractions[COLLISION] == 0 && interactable->activeInteractions[OVERLAP] == 0) {
						InteractionData_collision* interactionData = (InteractionData_collision*)malloc(sizeof(InteractionData_collision));
						if (NULL != interactionData) {
							interactionData->collisionNormal = contactNormal;
							interactionData->entityID = physicsBody->ENTITY_ID;
							interactionData->gameState = gameState;

							Interactable_interact(interactable, COLLISION, interactionData);
						}
					}
					InteractionData_overlap* overlapData = (InteractionData_overlap*)malloc(sizeof(InteractionData_overlap));
					if (NULL != overlapData) {
						overlapData->entityID = physicsBody->ENTITY_ID;

						Interactable_interact(interactable, OVERLAP, overlapData);
					}
				}

				if (staticCollider->type == STATIC) {
					Vec2_increaseByVec(&physicsBody->velocity,
						Vec2_imul((Vec2) { contactNormal.x* fabs(physicsBody->velocity.x), contactNormal.y* fabs(physicsBody->velocity.y) }, 1 - tHitNear));

					if (movementController != NULL) 
						movementController->collisionNormal = contactNormal;
				}
			}
		}
	} 
	Position_moveBy(position, Vec2_imul(physicsBody->velocity, deltaT));

	ECS_freeEntity(dynamicComps);
}
