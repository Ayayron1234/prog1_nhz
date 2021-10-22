#pragma once
#include <stdio.h>
#include "Tilemap.h"


typedef struct Sprite {
	int ENTITY_ID;

	Tilemap* tilemap;
	Vec2Int tilePosition;

	RenderProperties renderProps;
} Sprite;

void Sprite_init(Sprite* dest, int entityID, int* total_spriteComponents, Tilemap* tilemap, Vec2Int tilePosition);
void Sprite_deserialise(Sprite* sprite, int* total_spriteComponents, int maxNumberOfComponents, char path[255], Tilemap* tilemap);
void Sprite_serialise(Sprite* sprites, int maxNumberOfComponents, char path[255]);
