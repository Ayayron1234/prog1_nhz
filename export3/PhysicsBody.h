#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

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

#endif // !PHYSICSBODY_H
