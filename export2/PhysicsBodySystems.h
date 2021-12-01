#ifndef PHYSICSBODYSYSTEMS_H
#define PHYSICSBODYSYSTEMS_H

#include "ECS.h"
#include "PositionSystems.h"
#include "ColliderSystems.h"
#include "InteractableSystems.h"


void PhysicsBody_update(Layout* currentLayout, PhysicsBody* physicsBody, double deltaT, GameState* gameState);

#endif // !PHYSICSBODYSYSTEMS_H
