#ifndef MOVEMENTCONTROLLER_H
#define MOVEMENTCONTROLLER_H

#include <stdbool.h>
#include "Math.h"

typedef enum MovementStyle {
	PLATFORMER = 0,
	GOOMBA = 1,
} MovementStyle;

typedef struct MovementController {
	int ENTITY_ID;

	MovementStyle movementStyle;
	Vec2 collisionNormal;

	Vec2 previousDirection;
	double maxSpeed;
	double acceleration;
	double deceleration;

	double maxFallSpeed;
	double gravity;

	double jumpHeight;
	double timeToApex;
	bool doubleJump;
} MovementController;

#endif // !MOVEMENTCONTROLLER_H
