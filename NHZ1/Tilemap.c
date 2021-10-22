#include "Tilemap.h"


void Tilemap_init(Tilemap* dst, Vec2Int tileSize, SDL_Renderer* renderer, char path[255])
{
	dst->texture = IMG_LoadTexture(renderer, path);
	dst->tileSize = tileSize;
	if (dst->texture == NULL) {
		SDL_Log("SDL couldn't load %s texture: %s", path, IMG_GetError());
		return;
	}
}

SDL_Rect Tilemap_getSourceRect(Tilemap* tilemap, Vec2Int tilePosition) {
	SDL_Rect srcrect = {
		tilePosition.x * tilemap->tileSize.x, tilePosition.y * tilemap->tileSize.y,
		tilemap->tileSize.x, tilemap->tileSize.y
	};
	return srcrect;
}
