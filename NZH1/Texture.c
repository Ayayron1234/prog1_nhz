#include <stdio.h>
#include "Texture.h"


Texture Texture_init(SDL_Renderer* renderer, char path[255], bool isVisisble)
{
	Texture texture = { .size = { 0, 0 } };
	texture.image = IMG_LoadTexture(renderer, path);
	if (texture.image == NULL) {
		SDL_Log("SDL couldn't load %s texture: %s", path, IMG_GetError());
		return;
	}

	SDL_QueryTexture(texture.image, NULL, NULL, &texture.size.x, &texture.size.y);
	texture.isVisible = isVisisble;

	return texture;
}

void Texture_destroy(Texture* texture) {
	SDL_DestroyTexture(texture->image);
}

void Texture_render(SDL_Renderer* renderer, Texture* texture, Vec2 position)
{
	if (texture->isVisible) {
		SDL_Rect dstrect = { position.x, position.y, texture->size.x, texture->size.y };

		SDL_RenderCopy(renderer, texture->image, NULL, &dstrect);
	}
}
