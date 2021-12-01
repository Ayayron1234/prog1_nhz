#include "MovementControllerSystems.h"

void MovementController_update(Layout* currentLayout, MovementController* movementController, double delta_t) {
	switch (movementController->movementStyle)
	{
	case PLATFORMER: {
		PhysicsBody* physicsBody = ECS_getComponent(PHYSICS_BODY, *currentLayout, movementController->ENTITY_ID);
		if (physicsBody == NULL) break;

		//double g = ((2 * movementController->jumpHeight) / (pow(movementController->timeToApex, 2))) / physicsBody->mass;
		double g = ((movementController->jumpHeight) / (2 * pow(movementController->timeToApex, 2))) / physicsBody->mass;
		physicsBody->gravitationalAcceleration = (Vec2){ 0, g };

		SDL_PumpEvents();
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] != 0) { 
			if (movementController->collisionNormal.y < 0) {
				physicsBody->velocity.y = -sqrt(2 * 48 * g * movementController->jumpHeight);
				//physicsBody->velocity.y = -sqrt(2 * g * movementController->jumpHeight);
			}
		}
		else {
			if (physicsBody->velocity.y < 0) {
				physicsBody->velocity.y *= 0.95;
			}
		}
		//void** entity = ECS_getEntity(*currentLayout, movementController->ENTITY_ID);
		//if (NULL != mario) {
		//	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] != 0) {
		//		((PhysicsBody*)mario[PHYSICS_BODY])->velocity.x = -300;
		//	}
		//	else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] != 0) {
		//		((PhysicsBody*)mario[PHYSICS_BODY])->velocity.x = 300;
		//	}
		//	else {
		//		((PhysicsBody*)mario[PHYSICS_BODY])->velocity.x = 0;
		//	}
		//	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] != 0) {
		//		Vec2 pos = ((Position*)mario[POSITION])->value;
		//		Vec2 size = ((CollisionBox*)mario[COLLISION_BOX])->size;
		//		Vec2 a = { pos.x, pos.y + size.y + 1 };
		//		Vec2 b = { pos.x + size.x, pos.y + size.y + 1 };

		//		bool isOnFloor = false;
		//		for (int i = 0; i < ECS_getNumberOfComponents(COLLIDER, *game->currentLayout); i++) {
		//			CollisionBox* box = ECS_getComponent(COLLISION_BOX, *game->currentLayout, ((CollisionBox*)ECS_getNthComponent(COLLIDER, game->currentLayout, i))->ENTITY_ID);
		//			if (NULL == box) continue;
		//			if (CollisionBox_isPointInside(game->currentLayout, box, a) || CollisionBox_isPointInside(game->currentLayout, box, b)) isOnFloor = true;
		//		}
		//		if (isOnFloor)
		//			((PhysicsBody*)mario[PHYSICS_BODY])->velocity.y = -800;
		//	}
		//	int windowWidth;
		//	SDL_GetWindowSize(game->window, &windowWidth, NULL);

		//	Position* marioPosition = mario[POSITION];
		//	if ((game->currentLayout->camera.x + windowWidth / 2) < marioPosition->value.x - windowWidth / 8)
		//		game->currentLayout->camera = (Vec2){ marioPosition->value.x - windowWidth / 2 - windowWidth / 8, 0 };
		//	if ((game->currentLayout->camera.x + windowWidth / 2) > marioPosition->value.x + windowWidth / 8 && game->currentLayout->camera.x > 0)
		//		game->currentLayout->camera = (Vec2){ marioPosition->value.x - windowWidth / 2 + windowWidth / 8, 0 };
		//	if (game->currentLayout->camera.x < 0) game->currentLayout->camera = (Vec2){ 0, 0 };

		//	ECS_freeEntity(mario);
		//}

		break;
	}
	case GOOMBA: {
		PhysicsBody* physicsBody = ECS_getComponent(PHYSICS_BODY, *currentLayout, movementController->ENTITY_ID);
		if (NULL == physicsBody) break;

		if (physicsBody->velocity.x == 0) {
			if (movementController->previousDirection.x <= 0 || fabs(movementController->previousDirection.x) < 1);
				physicsBody->velocity.x = movementController->maxSpeed;
			if (movementController->previousDirection.x > 0)
				physicsBody->velocity.x = -movementController->maxSpeed;
		}

		movementController->previousDirection = physicsBody->velocity;
		break;
	}
	default:
		break;
	}
}