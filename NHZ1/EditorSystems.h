#pragma once
#include "ECS.h"

void Editor_render(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);
void Editor_renderForSprite(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);
void Editor_renderForTile(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);

int Editor_getSelected(ComponentLists* components);
void Editor_select(ComponentLists* components, int entityID);
void Editor_deselectAll(ComponentLists* components);
