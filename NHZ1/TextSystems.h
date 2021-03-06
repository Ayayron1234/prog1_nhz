#ifndef TEXTSYSTEMS_H
#define TEXTSYSTEMS_H

#include "SDL.h"
#include "SDL_ttf.h"
#include "ECS.h"
#include "Text.h"

void Text_render(Layout* currentLayout, Text* text, SDL_Renderer* renderer, TTF_Font* font);

#endif // !TEXTSYSTEMS_H
