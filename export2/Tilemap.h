#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdbool.h>
#include "Math.h"
#include "SDL.h"
#include "SDL_image.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


/**
Details of a component's rendering. 
 */
typedef struct RenderProperties {
	bool isVisible;			///< The component is only rendered when this is set to true. 
	double opacity;			///< The opacity of an entity
	double scale;			///< The scale of rendering. 
} RenderProperties;

/**
 Stores the texture of all entities. 
 */
typedef struct Tilemap {
	SDL_Texture *texture;	///< The texture which will be used in rendering the entities. 
	
	Vec2Int tileSize;		///< The dimensions of a tile. 
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

#endif

