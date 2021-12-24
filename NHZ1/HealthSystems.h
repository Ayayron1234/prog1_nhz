#ifndef HEALTHSYSTEMS_H
#define HEALTHSYSTEMS_H

#include "ECS.h"

void Health_update(Layout* layouts, int numberOfLayouts, char* layoutName, Health* health, double delta_t, GameState* gameState);


#endif // !HEALTHSYSTEMS_H
