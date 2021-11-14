#pragma once
#include "ECS.h"
#include "PositionSystems.h"
#include "ColliderSystems.h"

void PhysicsBody_update(ComponentLists* components, PhysicsBody* physicsBody, double deltaT);
