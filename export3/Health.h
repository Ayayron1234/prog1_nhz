#ifndef HEALTH_H
#define HEALTH_H

#include <stdbool.h>


typedef struct Health {
	int ENTITY_ID;

	double value;
	bool isInvulnerable;
	double timeSinceLastDamage;
} Health;

#endif // !HEALTH_H
