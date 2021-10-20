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

ComponentLists ECS_init(bool doDeserialisation);

int ECS_createEntity(int* ENTITIES);
void ECS_serialiseAllComponents(ComponentLists* components, char directory[255]);
void ECS_deserialiseAllComponents(ComponentLists* components, char *directory);

Texture* ECS_getTextureComponent(ComponentLists* components, int entityID);
int ECS_attachTextureComponent(ComponentLists* components, int entityID, Texture* texture);
void ECS_serialiseTextureComponents(ComponentLists* components, char filePath[255]);
void ECS_deserialiseTextureComponents(ComponentLists* components, char filePath[255]);

Position* ECS_getPositionComponent(ComponentLists* components, int entityID);
int ECS_attachPositionComponent(ComponentLists* components, int entityID, Position* position);
void ECS_serialisePositionComponents(ComponentLists* components, char filePath[255]);
void ECS_deserialisePositionComponents(ComponentLists* components, char filePath[255]);


#endif
