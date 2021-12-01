#include "TextSystems.h"


void Text_render(Layout* currentLayout, Text* text, SDL_Renderer* renderer, TTF_Font* ttfFont) {
	// get position of text
	Position* position = ECS_getComponent(POSITION, *currentLayout, text->ENTITY_ID);
	if (NULL == position) {
		printf("To render a text, the text's parent entity has to have a position component. ");
		exit(1);
	}

	Vec2 parallax = { 0, 0 };
	EntityRenderer* entityRenderer = ECS_getComponent(ENTITY_RENDERER, *currentLayout, text->ENTITY_ID);
	if (NULL != entityRenderer) parallax = currentLayout->layers[entityRenderer->layerIndex].parallax;

	// create properties
	TTF_Font* font = (NULL != ttfFont) ? ttfFont : TTF_OpenFont(text->fontFamily, text->fontSize);
	SDL_Color color = { text->fontColor.r, text->fontColor.g, text->fontColor.b };


	// initiate surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, (strlen(text->value) > 0) ? text->value : " ", color);
	text->textBoxSize.x = textSurface->w;
	text->textBoxSize.y = textSurface->h;

	// render background
	Interactable* interactable = ECS_getComponent(INTERACTABLE, *currentLayout, text->ENTITY_ID);
	if (NULL != interactable) {
		int border = 2;
		int padding = 6;
		if (interactable->type == TEXTBOX) {
			SDL_Rect rect = { .x = (position->value.x - padding) - currentLayout->camera.x * parallax.x, .y = (position->value.y - padding) - currentLayout->camera.y * parallax.y,
							  .w = textSurface->w + padding * 2, .h = textSurface->h + padding * 2 };
			SDL_Rect outerRect = { .x = (position->value.x - padding) - currentLayout->camera.x * parallax.x, .y = (position->value.y - padding) - currentLayout->camera.y * parallax.y,
								   .w = textSurface->w + padding * 2, .h = textSurface->h + padding * 2 + border };
			text->textBoxSize.x += padding;
			text->textBoxSize.y += padding;

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			if (interactable->hasFocus)
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			SDL_RenderFillRect(renderer, &outerRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
		else if (interactable->type == BUTTON) {
			SDL_Rect rect = { .x = (position->value.x - padding) - currentLayout->camera.x * parallax.x, .y = (position->value.y - padding) - currentLayout->camera.y * parallax.y,
				  .w = textSurface->w + padding * 2, .h = textSurface->h + padding * 2 };
			SDL_Rect outerRect = { .x = (position->value.x - padding - border) - currentLayout->camera.x * parallax.x, .y = (position->value.y - padding - border) - currentLayout->camera.y * parallax.y,
								   .w = textSurface->w + padding * 2 + border * 2, .h = textSurface->h + padding * 2 + border * 2};
			text->textBoxSize.x += padding;
			text->textBoxSize.y += padding;

			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			if (interactable->hasFocus)
				SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
			SDL_RenderFillRect(renderer, &outerRect);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}

	// convert surface to texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	// init textbox
	SDL_Rect Message_rect = { 
		.x = position->value.x - currentLayout->camera.x * parallax.x, .y = position->value.y - currentLayout->camera.y * parallax.y,
		.w = textSurface->w, .h = textSurface->h };

	// render texture
	SDL_RenderCopy(renderer, textTexture, NULL, &Message_rect);

	// free texture and surface
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
}
