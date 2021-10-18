#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Vec2.h"


typedef struct Texture {
	int ENTITY_ID;

	SDL_Texture *image;
	Vec2Int size;

	bool isVisible;
} Texture;

Texture Texture_init(SDL_Renderer *renderer, char path[255], bool isVisible);

void Texture_destroy(Texture* texture);

void Texture_render(SDL_Renderer* renderer, Texture* texture, Vec2 position);

#endif
