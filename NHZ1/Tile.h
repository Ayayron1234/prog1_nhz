#pragma once
#include <stdio.h>
#include "Vec2.h"
#include "Tilemap.h"
#include "misc.h"


typedef struct Tile {
	int ENTITY_ID;

	Tilemap* tilemap;
	Vec2Int tilePosition;

	Vec2Int size;
	RenderProperties renderProps;
} Tile;

void Tile_init(Tile* tiles, int entityID, int* total_tileComponents, Tilemap* tilemap, Vec2Int tilePosition, Vec2Int size);
void Tile_deserialise(Tile* tiles, int* total_tileComponents, int maxNumberOfComponents, char path[255], Tilemap* tilemap);
void Tile_serialise(Tile* tiles, int maxNumberOfComponents, char path[255]);
