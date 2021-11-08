#pragma once
#include <stdbool.h>
#include "ECS.h"
#include "SDL.h"
#include "SDL_ttf.h"

void Editor_render(int gameState, ComponentLists* components, Editor* editor, SDL_Renderer* renderer);
//void Editor_renderForSprite(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);
//void Editor_renderForTile(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);
void Editor_renderPositionData(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);

int Editor_getSelected(ComponentLists* components);
void Editor_select(ComponentLists* components, int entityID);
void Editor_deselectAll(ComponentLists* components);

void Editor_copy(ComponentLists* components, int entityID);
void Editor_paste(ComponentLists* components, int newEntityID);