#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Math.h"


typedef struct PhysicsBody {
	int ENTITY_ID;
	
	double mass;
	Vec2 gravitationalAcceleration;
	Vec2 velocity;
	Vec2 acceleration;
} PhysicsBody;

void PhysicsBody_init(PhysicsBody* physicsBodies, int entityID, int* total_physicsBodyComponents, double mass);
void PhysicsBody_delete(PhysicsBody* physicsBodies, int entityID, int* total_physicsBodyComponents);

void PhysicsBody_deserialise(PhysicsBody* physicsBodies, int* total_physicsBodyComponents, int maxNumberOfComponents, char path[255]);
void PhysicsBody_serialise(PhysicsBody* physicsBodies, int maxNumberOfComponents, char path[255]);

