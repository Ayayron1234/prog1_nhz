#pragma once
#include <stdbool.h>
#include "ECS.h"
#include "CollisionBoxSystems.h"

bool Collider_checkForCollision(Layout* currentLayoutW, Collider* dynamicC, Collider* staticC, Vec2* contactPoint, Vec2* contactNormal, double* tHitNear, double deltaT);