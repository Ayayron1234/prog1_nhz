#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "Math.h"


typedef struct CollisionBox {
	int ENTITY_ID;

	Vec2 size;
} CollisionBox;

void CollisionBox_init(CollisionBox* collisionBoxes, int entityID, int* total_collisionBoxComponents, Vec2 size);
void CollisionBox_delete(CollisionBox* collisionBoxes, int entityID, int* total_collisionBoxComponents);

void CollisionBox_deserialise(CollisionBox* collisionBoxes, int* total_collisionBoxComponents, int maxNumberOfComponents, char path[255]);
void CollisionBox_serialise(CollisionBox* collisionBoxes, int maxNumberOfComponents, char path[255]);
