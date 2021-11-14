#pragma once
#include <stdbool.h>
#include "Math.h"
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

/**
* Loads tilemap from the specified path and sets it's dimensions. 
* @param dst Pointer to the tilemap where the data should be loaded.
* @param tileSize dimensions of an object in this tilemap. 
* @param renderer SDL renderer where the tilemap will be rendered.
* @param path Path of the image file containing the tilemap. 
*/
void Tilemap_init(Tilemap* dst, Vec2Int tileSize, SDL_Renderer* renderer, char path[255]);

/**
* Returns the position and dimensions of a given tile. 
* @param tilemap The tilemap from which we want to get the tile's position and dimensions. 
* @param tilePosition the two dimensional index of where the tile is inside the tile map. This is not the same as the position that is returned by this function.
* @returns A rectangle, which corresponds to the part of the tilemap which contains the tile that we are looking for. 
*/
SDL_Rect Tilemap_getSourceRect(Tilemap* tilemap, Vec2Int tilePosition);
