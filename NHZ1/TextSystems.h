#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include "ECS.h"
#include "Text.h"

void Text_render(ComponentLists* components, Text* text, SDL_Renderer* renderer);

