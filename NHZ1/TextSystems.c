#include "TextSystems.h"


void Text_render(Layout* currentLayout, Text* text, SDL_Renderer* renderer, TTF_Font* ttfFont) {
	// get position of text
	Position* position = ECS_getComponent(POSITION, *currentLayout, text->ENTITY_ID);
	if (NULL == position) {
		printf("To render a text, the text's parent entity has to have a position component. ");
		exit(1);
	}

	// create properties
	TTF_Font* font = (NULL == ttfFont) ? ttfFont : TTF_OpenFont(text->fontFamily, text->fontSize);
	SDL_Color color = { text->fontColor.r, text->fontColor.g, text->fontColor.b };

	// initiate surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text->value, color);

	// convert surface to texture
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

	// init textbox
	SDL_Rect Message_rect = { 
		.x = position->value.x, .y = position->value.y, 
		.w = textSurface->w, .h = textSurface->h };

	// render texture
	SDL_RenderCopy(renderer, textTexture, NULL, &Message_rect);

	// free texture and surface
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);
	TTF_CloseFont(font);
}
