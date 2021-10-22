#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL.h"
#include "Vec2.h"


typedef struct Animation {
	int ENTITY_ID;

	Vec2Int tilePosition;
	int currentFrame;
	int frameCount;

	Uint32 lastUpdateTime;
	double animationSpeed;
} Animation;

void Animation_init(Animation* animations, int entity_ID, int* total_animationComponents, Vec2Int tilePosition, int frameCount, double animationSpeed);
void Animation_deserialise(Animation* animations, int* total_animationComponents, int maxNumberOfComponents, char path[255]);
void Animation_serialise(Animation* animations, int maxNumberOfComponents, char path[255]);
