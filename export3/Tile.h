#ifndef TILE_H
#define TILE_H

#include <stdio.h>
#include "Math.h"
#include "Tilemap.h"
#include "misc.h"


typedef struct Tile {
	int ENTITY_ID;

	Tilemap* tilemap;
	Vec2Int tilePosition;

	Vec2Int size;
	RenderProperties renderProps;
} Tile;

#endif // !TILE
