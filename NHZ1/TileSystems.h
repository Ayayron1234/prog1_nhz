#pragma once
#include "ECS.h"
#include "Tile.h"
#include "Position.h"
#include "EditorSystems.h"


void Tile_render(Layout* currentLayout, Tile* tile, SDL_Renderer* renderer);
//bool Tile_isPointInside(ComponentLists* components, Tile* tile, Vec2 point);
//
//void Tile_edit_update(ComponentLists* components, Tile* tile, SDL_Renderer* renderer);
