#pragma once
#include <stdbool.h>
#include "ECS.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "CollisionBoxSystems.h"

void Editor_render(int gameState, Layout* currentLayout, Editor* editor, SDL_Renderer* renderer);
//void Editor_renderForSprite(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);
//void Editor_renderForTile(ComponentLists* components, Editor* editor, SDL_Renderer* renderer);
void Editor_renderPositionData(Layout* currentLayout, Editor* editor, SDL_Renderer* renderer);

void Editor_update(GameState gameState, Layout* currentLayout, Editor* editor);

int Editor_getSelected(Layout* currentLayout);
void Editor_select(Layout* currentLayout, int entityID);
void Editor_deselectAll(Layout* currentLayout);
void Editor_selectEntityAtClick(Layout* currentLayout, Vec2 mousePosition);

void Editor_copy(Layout* currentLayout, int entityID);
void Editor_paste(Layout* layouts, int numberOfLayouts, char* layoutName);