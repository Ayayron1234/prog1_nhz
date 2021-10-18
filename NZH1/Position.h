#ifndef POSITION_H
#define POSITION_H

#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>
#include "Vec2.h"


typedef struct Position {
	int ENTITY_ID;

	Vec2 value;
} Position;

Position Position_init(Vec2 value);

void Position_moveBy(Position* position, Vec2 amount);


#endif
