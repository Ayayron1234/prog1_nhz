#pragma once
#include <stdbool.h>
#include "SDL.h"
#include "ECS.h"
#include "EditorSystems.h"
#include "Math.h"


typedef struct Ray2 {
	Vec2 origin;
	Vec2 destination;
} Ray2;


// these functions will change in the future. 
void CollisionBox_update(int gameState, ComponentLists* components, CollisionBox* collisionBox);

bool CollisionBox_isPointInside(Layout* currentLayout, CollisionBox* collisionBox, Vec2 point);
bool CollisionBox_checkForOverlapp(ComponentLists* components, CollisionBox* collisionBoxA, CollisionBox* collisionBoxB);
bool CollisionBox_checkIfRayOverlapps(Layout* currentLayout, CollisionBox* collisionBox, Ray2 ray, Vec2* contactPoint, Vec2* contactNormal, double* tHitNear);
