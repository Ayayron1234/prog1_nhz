#pragma once
#include "Sprite.h"
#include "Position.h"
#include "EditorSystems.h"
#include "ECS.h"


void Sprite_render(Layout* currentLayout, Sprite* sprite, SDL_Renderer* renderer);
//bool Sprite_isPointInside(ComponentLists* components, Sprite* sprite, Vec2 point);
//
//void Sprite_edit_update(ComponentLists* components, Sprite* sprite, SDL_Renderer* renderer);
