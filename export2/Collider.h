#ifndef COLLIDER_H
#define COLLIDER_H

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

#endif // !COLLIDER_H
