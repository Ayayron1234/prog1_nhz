#pragma once
#include <stdbool.h>
#include "Vec2.h"
#include "SDL.h"
#include "SDL_image.h"


typedef struct RenderProperties {
	bool isVisible;
	double opacity;
	double scale;
} RenderProperties;

typedef struct Tilemap {
	SDL_Texture *texture;
	
	Vec2Int tileSize;
} Tilemap;


void Tilemap_init(Tilemap* dst, Vec2Int tileSize, SDL_Renderer* renderer, char path[255]);

SDL_Rect Tilemap_getSourceRect(Tilemap* tilemap, Vec2Int tilePosition);
