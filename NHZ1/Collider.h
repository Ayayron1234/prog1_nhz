#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "Math.h"


typedef int Collider_colliderType;
typedef enum {
	STATIC = 0, 
	DYNAMIC = 1
} Collider_colliderType;

typedef struct Collider {
	int ENTITY_ID;

	Collider_colliderType type;
} Collider;

void Collider_init(Collider* colliders, int entityID, int* total_colliderComponents, bool isDynamic);
void Collider_delete(Collider* colliders, int entityID, int* total_colliderComponents);

void Collider_deserialise(Collider* colliders, int* total_colliderComponents, int maxNumberOfComponents, char path[255]);
void Collider_serialise(Collider* colliders, int maxNumberOfComponents, char path[255]);
