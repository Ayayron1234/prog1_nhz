#ifndef COLLISIONBOX_H
#define COLLISIONBOX_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "Math.h"

/**
Stores the size of the entity. This components is mainly, but not exclusively used for collision and click detection. 
*/
typedef struct CollisionBox {
	int ENTITY_ID;

	Vec2 size;
} CollisionBox;

#endif // !COLLISIONBOX_H
