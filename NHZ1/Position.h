#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include "Vec2.h"


typedef struct Position {
	int ENTITY_ID;
	Vec2 value;
} Position;

Position* Position_init(Position* dest, Vec2 value);
void Position_deserialise(Position* position, int* total_PositionComponents, int maxNumberOfComponents, char path[255]);
void Position_serialise(Position* positions, int maxNumberOfComponents, char path[255]);

void Position_moveBy(Position* position, Vec2 amount);
