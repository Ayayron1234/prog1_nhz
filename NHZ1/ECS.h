#pragma once
#include <stdbool.h>
#include <string.h>
#include "Position.h"


typedef struct ComponentLists {
	int total_positionComponents;
	Position positionComponents[128];
} ComponentLists;

ComponentLists* ECS_init(int maxNumberOfComponents, bool doDeserialisation, char saveDirectory[255]);
void ECS_deserialiseAllComponents(ComponentLists* components, char* directory);
void ECS_serialiseAllComponents(ComponentLists* components, char* directory);
