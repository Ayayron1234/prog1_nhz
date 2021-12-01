#ifndef SPRITESYSTEMS_H
#define SPRITESYSTEMS_H

#include "Sprite.h"
#include "Position.h"
#include "EditorSystems.h"
#include "ECS.h"


void Sprite_render(Layout* currentLayout, Sprite* sprite, SDL_Renderer* renderer);

#endif // !SPRITESYSTEMS_H
