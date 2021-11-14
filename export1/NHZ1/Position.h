#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include "Math.h"


typedef struct Position {
	int ENTITY_ID;
	Vec2 value;
} Position;

void Position_init(int entityID, int* total_positionComponents, Position* dest, Vec2 value);
void Position_delete(int entityID, int* total_positionComponents, Position* positions);

void Position_deserialise(Position* positions, int* total_PositionComponents, int maxNumberOfComponents, char path[255]);
void Position_serialise(Position* positions, int maxNumberOfComponents, char path[255]);

