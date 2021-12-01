#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "SDL.h"
#include "Math.h"


typedef struct Animation {
	int ENTITY_ID;

	Vec2Int tilePosition;
	int currentFrame;
	int frameCount;

	Uint32 lastUpdateTime;
	double animationSpeed;
} Animation;

#endif // !ANIMATION_H
