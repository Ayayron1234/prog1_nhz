#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <stdbool.h>
#include "Tilemap.h"


typedef struct Sprite {
	int ENTITY_ID;

	Tilemap* tilemap;
	Vec2Int tilePosition;

	RenderProperties renderProps;
} Sprite;

#endif // !SPRITE_H
