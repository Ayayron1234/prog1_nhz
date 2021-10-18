#include <SDL.h>
#include "ECS.h"


Texture* ECS_getTextureComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_textureComponents; i++) {
		if (components->textureComponents[i].ENTITY_ID == entityID)
			return &components->textureComponents[i];
	}
}
int ECS_attachTextureComponent(ComponentLists* components, int entityID, Texture* texture)
{
	if (components->total_textureComponents > 254) {
		SDL_Log("Total number of texture components has reached the maximum!");
		return 1;
	}
	texture->ENTITY_ID = entityID;
	components->textureComponents[components->total_textureComponents++] = *texture;

	return 0;
}

Position* ECS_getPositionComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_positionComponents; i++) {
		if (components->positionComponents[i].ENTITY_ID == entityID) 
			return &components->positionComponents[i];		
	}
	return NULL;
}
int ECS_attachPositionComponent(ComponentLists* components, int entityID, Position* position)
{
	if (components->total_positionComponents > 254) {
		SDL_Log("Total number of position components has reached the maximum!");
		return 1;
	}
	position->ENTITY_ID = entityID;
	components->positionComponents[components->total_positionComponents++] = *position;

	return 0;
}

int ECS_createEntity(int* ENTITIES)
{
	int id = (*ENTITIES)++;
	return id;
}
