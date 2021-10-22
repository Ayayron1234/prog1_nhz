#pragma warning(disable : 4996)
#include "ECS.h"

ComponentLists ECS_init(int maxNumberOfComponents, bool doDeserialisation, char saveDirectory[255], Tilemap *tilemap)
{
	// allocate memory for the position components
	int total_positionComponents = 0;
	Position* positionComponents = (Position*)calloc(maxNumberOfComponents, sizeof(Position));
	if (NULL == positionComponents) exit(1);
	// allocate memory for the sprite components
	int total_spriteComponents = 0;
	Sprite* spriteComponents = (Sprite*)calloc(maxNumberOfComponents, sizeof(Sprite));
	if (NULL == spriteComponents) exit(1);
	// allocate memory for the editor components
	int total_editorComponents = 0;
	Editor* editorComponents = (Editor*)calloc(maxNumberOfComponents, sizeof(Editor));
	if (NULL == editorComponents) exit(1);
	// allocate memory for the animation components
	int total_animationComponents = 0;
	Animation* animationComponents = (Animation*)calloc(maxNumberOfComponents, sizeof(Animation));
	if (NULL == animationComponents) exit(1);
	// allocate memory for the tile components
	int total_tileComponents = 0;
	Tile* tileComponents = (Tile*)calloc(maxNumberOfComponents, sizeof(Tile));
	if (NULL == animationComponents) exit(1);

	if (doDeserialisation) {
		char path[255];

		strcpy(path, saveDirectory);
		Position_deserialise(positionComponents, &total_positionComponents, maxNumberOfComponents, strcat(path, "position.data"));
		strcpy(path, saveDirectory);
		Sprite_deserialise(spriteComponents, &total_spriteComponents, maxNumberOfComponents, strcat(path, "sprite.data"), tilemap);
		strcpy(path, saveDirectory);
		Editor_deserialise(editorComponents, &total_editorComponents, maxNumberOfComponents, strcat(path, "editor.data"));
		strcpy(path, saveDirectory);
		Animation_deserialise(animationComponents, &total_animationComponents, maxNumberOfComponents, strcat(path, "animation.data"));
		strcpy(path, saveDirectory);
		Tile_deserialise(tileComponents, &total_tileComponents, maxNumberOfComponents, strcat(path, "tile.data"), tilemap);
	}

	ComponentLists components = {
		.total_positionComponents = total_positionComponents,
		.positionComponents = positionComponents,
		.total_spriteComponents = total_spriteComponents,
		.spriteComponents = spriteComponents,
		.total_editorComponents = total_editorComponents,
		.editorComponents = editorComponents,
		.total_animationComponents = total_animationComponents,
		.animationComponents = animationComponents,
		.total_tileComponents = total_tileComponents,
		.tileComponents = tileComponents

	};
	return components;
}

int ECS_createEntity(int* ENTITIES)
{
	return ++(*ENTITIES);
}
void ECS_printEntityData(ComponentLists* components, int entityID) {
	printf("\n\nEntity #%d", entityID);
	
	Position* pos = ECS_getPositionComponent(components, entityID);
	if (pos != NULL) 
		printf("\n   >Position\n     .value=Vec2(%g, %g)", pos->value.x, pos->value.y);
	Sprite* sprite = ECS_getSpriteComponent(components, entityID);
	if (sprite != NULL) 
		printf("\n   >Sprite\n     .tilePosition=Vec2Int(%d, %d)", sprite->tilePosition.x, sprite->tilePosition.y);
	Animation* animation = ECS_getAnimationComponent(components, entityID);
	if (animation != NULL)
		printf("\n   >Animation\n     .tilePosition=Vec2Int(%d, %d)\n     .frameCount=%d\n     .animationSpeed=%g",
			animation->tilePosition.x, animation->tilePosition.y, animation->frameCount, animation->animationSpeed);
	Tile* tile = ECS_getTileComponent(components, entityID);
	if (tile != NULL)
		printf("\n   >Tile\n     .tilePosition=Vec2Int(%d, %d)\n     .size=Vec2Int(%d, %d)\n",
			tile->tilePosition.x, tile->tilePosition.y, tile->size.x, tile->size.y);
}

Position* ECS_getPositionComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_positionComponents; i++) {
		if (components->positionComponents[i].ENTITY_ID == entityID)
			return &components->positionComponents[i];
	}
	return NULL;
}
Sprite* ECS_getSpriteComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_spriteComponents; i++) {
		if (components->spriteComponents[i].ENTITY_ID == entityID)
			return &components->spriteComponents[i];
	}
	return NULL;
}
Editor* ECS_getEditorComponent(ComponentLists* components, int entityID) {
	for (int i = 0; i < components->total_editorComponents; i++) {
		if (components->editorComponents[i].ENTITY_ID == entityID)
			return &components->editorComponents[i];
	}
	return NULL;
}
Animation* ECS_getAnimationComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_animationComponents; i++) {
		if (components->animationComponents[i].ENTITY_ID == entityID)
			return &components->animationComponents[i];
	}
	return NULL;
}
Tile* ECS_getTileComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_tileComponents; i++) {
		if (components->tileComponents[i].ENTITY_ID == entityID)
			return &components->tileComponents[i];
	}
	return NULL;
}

