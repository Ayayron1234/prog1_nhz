#ifndef ECS_H
#define ECS_H

#include <stdio.h>
#include "Texture.h"
#include "Position.h"


typedef struct ComponentLists {
	int total_textureComponents;
	Texture textureComponents[255];
	int total_positionComponents;
	Position positionComponents[255];
} ComponentLists;

int ECS_createEntity(int* ENTITIES);

Texture* ECS_getTextureComponent(ComponentLists* components, int entityID);
int ECS_attachTextureComponent(ComponentLists* components, int entityID, Texture* texture);

Position* ECS_getPositionComponent(ComponentLists* components, int entityID);
int ECS_attachPositionComponent(ComponentLists* components, int entityID, Position* position);


#endif
