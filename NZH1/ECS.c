#pragma warning(disable : 4996)
#include <SDL.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>
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
void ECS_serialiseTextureComponents(ComponentLists* components, char filePath[255])
{
}
void ECS_deserialiseTextureComponents(ComponentLists* components, char filePath[255])
{
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
void ECS_serialisePositionComponents(ComponentLists* components, char filePath[255])
{
	SDL_Log("Saving Position components to file: %s...", filePath);
	FILE* f;
	fopen_s(&f, filePath, "wb");
	if (f != 0) fwrite(&components->positionComponents, sizeof(struct Position), 255, f);
	if (f != 0) fclose(f);
}
void ECS_deserialisePositionComponents(ComponentLists* components, char filePath[255])
{
	SDL_Log("Loading Position components from file: %s...", filePath);
	FILE* ifp;
	fopen_s(&ifp, filePath, "rb");
	if (ifp != NULL) 
		if (ifp != 0) {
			fread(&(components->positionComponents), sizeof(struct Position), 255, ifp);
		}
	if (ifp != 0) fclose(ifp);
}

ComponentLists ECS_init(bool doDeserialisation)
{
	Texture* textures = (Texture*)calloc(255, sizeof(Texture));
	if (NULL == textures)
		exit(1);
	Position* positions = (Position*)calloc(255, sizeof(Position));
	if (NULL == positions)
		exit(1);

	ComponentLists components = {
		.total_textureComponents = 0,
		.textureComponents = textures,
		.total_positionComponents = 0,
		.positionComponents = positions,
	};

	return components;
}
void ECS_close(ComponentLists* components) {
	//free(components->positionComponents);
	//free(components->textureComponents);
}

int ECS_createEntity(int* ENTITIES)
{
	int id = (*ENTITIES)++;
	return id;
}

void ECS_serialiseAllComponents(ComponentLists* components, char directory[255])
{
	char full[255];
	// serialise position components
	strcpy(full, directory);
	ECS_serialisePositionComponents(&components, strcat(full, "position.data"));
	// serialise texture components
	strcpy(full, directory);
	ECS_serialiseTextureComponents(&components, strcat(full, "texture.data"));
}
void ECS_deserialiseAllComponents(ComponentLists* components, char *directory)
{
	char full[255];
	// deserialise texture components
	strcpy(full, directory);
	ECS_deserialiseTextureComponents(&components, strcat(full, "texture.data"));
	// deserialise position components
	strcpy(full, directory);
	ECS_deserialisePositionComponents(&components, strcat(full, "position.data"));
}
