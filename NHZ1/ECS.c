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
	if (NULL == tileComponents) exit(1);
	// allocate memory for the text components
	int total_textComponents = 0;
	Text* textComponents = (Text*)calloc(maxNumberOfComponents, sizeof(Text));
	if (NULL == textComponents) exit(1);
	// allocate memory for the collisionBox components
	int total_collisionBoxComponents = 0;
	CollisionBox* collisionBoxComponents = (CollisionBox*)calloc(maxNumberOfComponents, sizeof(CollisionBox));
	if (NULL == collisionBoxComponents) exit(1);
	// allocate memory for the collider components
	int total_colliderComponents = 0;
	Collider* colliderComponents = (Collider*)calloc(maxNumberOfComponents, sizeof(Collider));
	if (NULL == colliderComponents) exit(1);
	// allocate memory for the physicsBody components
	int total_physicsBodyComponents = 0;
	PhysicsBody* physicsBodyComponents = (PhysicsBody*)calloc(maxNumberOfComponents, sizeof(PhysicsBody));
	if (NULL == physicsBodyComponents) exit(1);


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
		strcpy(path, saveDirectory);
		Text_deserialise(textComponents, &total_textComponents, maxNumberOfComponents, strcat(path, "text.data"));
		strcpy(path, saveDirectory);
		CollisionBox_deserialise(collisionBoxComponents, &total_collisionBoxComponents, maxNumberOfComponents, strcat(path, "collisionBox.data"));
		strcpy(path, saveDirectory);
		Collider_deserialise(colliderComponents, &total_colliderComponents, maxNumberOfComponents, strcat(path, "collider.data"));
		strcpy(path, saveDirectory);
		PhysicsBody_deserialise(physicsBodyComponents, &total_physicsBodyComponents, maxNumberOfComponents, strcat(path, "physicsBody.data"));
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
		.tileComponents = tileComponents,
		.total_textComponents = total_textComponents,
		.textComponents = textComponents,
		.total_collisionBoxComponents = total_collisionBoxComponents,
		.collisionBoxComponents = collisionBoxComponents,
		.total_colliderComponents = total_colliderComponents,
		.colliderComponents = colliderComponents,
		.total_physicsBodyComponents = total_physicsBodyComponents,
		.physicsBodyComponents = physicsBodyComponents
	};
	return components;
}

int ECS_createEntity(ComponentLists* components, int maxNumberOfComponents) {
	int *freeIDs;
	freeIDs = (int*)calloc(maxNumberOfComponents + 1, sizeof(int));
	if (NULL == freeIDs) exit(1);
	for (int i = 0; i < maxNumberOfComponents; i++) freeIDs[i] = i;

	for (int i = 0; i < maxNumberOfComponents; i++) {
		freeIDs[components->positionComponents[i].ENTITY_ID ] = 0;
		freeIDs[components->spriteComponents[i].ENTITY_ID] = 0;
		freeIDs[components->animationComponents[i].ENTITY_ID] = 0;
		freeIDs[components->tileComponents[i].ENTITY_ID] = 0;
		freeIDs[components->textComponents[i].ENTITY_ID] = 0;
		freeIDs[components->editorComponents[i].ENTITY_ID] = 0;
		freeIDs[components->collisionBoxComponents[i].ENTITY_ID] = 0;
		freeIDs[components->colliderComponents[i].ENTITY_ID] = 0;
		freeIDs[components->physicsBodyComponents[i].ENTITY_ID] = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++)
		if (freeIDs[i] != 0) return freeIDs[i];
	return 0;
}
void ECS_deleteEntity(ComponentLists* components, int entityID) {
	Position_delete(entityID, &components->total_positionComponents, components->positionComponents);
	Sprite_delete(components->spriteComponents, entityID, &components->total_spriteComponents);
	Animation_delete(components->animationComponents, entityID, &components->total_animationComponents);
	Tile_delete(components->tileComponents, entityID, &components->total_tileComponents);
	Text_delete(components->textComponents, entityID, &components->total_textComponents);
	Editor_delete(components->editorComponents, entityID, &components->total_editorComponents);
	CollisionBox_delete(components->collisionBoxComponents, entityID, &components->total_collisionBoxComponents);
	Collider_delete(components->colliderComponents, entityID, &components->total_colliderComponents);
	PhysicsBody_delete(components->physicsBodyComponents, entityID, &components->total_physicsBodyComponents);
}

void ECS_printEntityData(ComponentLists* components, int entityID) {
	printf("\n\nEntity #%d", entityID);
	
	Position* pos = ECS_getPositionComponent(components, entityID);
	if (pos != NULL) 
		printf("\n   >Position\n     .value=Vec2(%g, %g)", pos->value.x, pos->value.y);
	CollisionBox* collisionBox = ECS_getCollisionBoxComponent(components, entityID);
	if (collisionBox != NULL)
		printf("\n   >CollisionBox\n     .size=Vec2(%g, %g)", collisionBox->size.x, collisionBox->size.y);
	Sprite* sprite = ECS_getSpriteComponent(components, entityID);
	if (sprite != NULL) 
		printf("\n   >Sprite\n     .tilePosition=Vec2Int(%d, %d)", sprite->tilePosition.x, sprite->tilePosition.y);
	Animation* animation = ECS_getAnimationComponent(components, entityID);
	if (animation != NULL)
		printf("\n   >Animation\n     .tilePosition=Vec2Int(%d, %d)\n     .frameCount=%d\n     .animationSpeed=%g",
			animation->tilePosition.x, animation->tilePosition.y, animation->frameCount, animation->animationSpeed);
	Tile* tile = ECS_getTileComponent(components, entityID);
	if (tile != NULL)
		printf("\n   >Tile\n     .tilePosition=Vec2Int(%d, %d)\n     .size=Vec2Int(%d, %d)",
			tile->tilePosition.x, tile->tilePosition.y, tile->size.x, tile->size.y);
	Text* text = ECS_getTextComponent(components, entityID);
	if (text != NULL)
		printf("\n   >Text\n     .value=%s\n     .textBoxSize=Vec2Int(%d, %d)\n     .fontColor=RGB(%d, %d, %d)",
			text->value, text->textBoxSize.x, text->textBoxSize.y, text->fontColor.r, text->fontColor.g, text->fontColor.b);
	Collider* collider = ECS_getColliderComponent(components, entityID);
	if (collider != NULL)
		printf("\n   >Collider\n     .type=%s", (collider->type == DYNAMIC) ? "DYNAMIC" : "STATIC");
	PhysicsBody* physicsBody = ECS_getPhysicsBodyComponent(components, entityID);
	if (physicsBody != NULL)
		printf("\n   >PhysicsBody\n     .mass=%g\n     .gravitationalAcceleration=Vec2(%g, %g)\n     .velocity=Vec2(%g, %g)\n     .acceleration=Vec2(%g, %g)", 
			physicsBody->mass, physicsBody->gravitationalAcceleration.x, physicsBody->gravitationalAcceleration.y, physicsBody->velocity.x, physicsBody->velocity.y,
			physicsBody->acceleration.x, physicsBody->acceleration.y);
	Editor* editor = ECS_getEditorComponent(components, entityID);
	if (editor != NULL)
		printf("\n   >Editor\n     .copied=%s\n", (editor->copied) ? "true": "false");
}

//void ECS_serialise(int nComponentLists, ComponentLists* components) {
//	SerialisationMapFragment componentsMap = { .total_components = 0, .componentSize = sizeof(Position) };
//	componentsMap.layoutMaps = (Layout*)calloc(nComponentLists, sizeof(Layout));
//
//	for (componentsMap.total_layouts = 0; componentsMap.total_layouts < nComponentLists; componentsMap.total_layouts++) {
//		componentsMap.layoutMaps[componentsMap.total_layouts].start = componentsMap.total_components;
//		componentsMap.total_components += components[componentsMap.total_layouts].total_positionComponents;
//		componentsMap.layoutMaps[componentsMap.total_layouts].end = componentsMap.total_components - 1;
//	}
//	
//	Position* sumComponents = (void*)calloc(componentsMap.total_components, componentsMap.componentSize);
//	if (NULL == sumComponents) exit(1);
//
//	for (int i = 0; i < componentsMap.total_layouts; i++) {
//		for (int j = 0; j < componentsMap.layoutMaps[i].end; j++) {
//			sumComponents[componentsMap.layoutMaps[i].start + j] = components[i].positionComponents[j];
//		}
//	}
//
//	return;
//}

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
Text* ECS_getTextComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_textComponents; i++) {
		if (components->textComponents[i].ENTITY_ID == entityID)
			return &components->textComponents[i];
	}
	return NULL;
}
CollisionBox* ECS_getCollisionBoxComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_collisionBoxComponents; i++) {
		if (components->collisionBoxComponents[i].ENTITY_ID == entityID)
			return &components->collisionBoxComponents[i];
	}
	return NULL;
}
Collider* ECS_getColliderComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_colliderComponents; i++) {
		if (components->colliderComponents[i].ENTITY_ID == entityID)
			return &components->colliderComponents[i];
	}
	return NULL;
}
PhysicsBody* ECS_getPhysicsBodyComponent(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_physicsBodyComponents; i++) {
		if (components->physicsBodyComponents[i].ENTITY_ID == entityID)
			return &components->physicsBodyComponents[i];
	}
	return NULL;
}

void ECS_deserialise(Layout** layouts, int numberOfLayouts, void*** componentLists, SerialisationMapFragment* serialisationMapFragments, int numberOfComponentTypes) {
	for (int layoutIndex = 0; layoutIndex < numberOfLayouts; layoutIndex++) {
		(*layouts)[layoutIndex].componentMaps = (LayoutMap*)calloc(NUMBER_OF_COMPONENT_TYPES, sizeof(LayoutMap));

		if (NULL == (*layouts)[layoutIndex].componentMaps) exit(1);

		for (int componentTypeIndex = 0; componentTypeIndex < numberOfComponentTypes; componentTypeIndex++) {
			(*layouts)[layoutIndex].componentListsPointers[componentTypeIndex] = (*componentLists)[componentTypeIndex];
			(*layouts)[layoutIndex].componentMaps[componentTypeIndex] = serialisationMapFragments[componentTypeIndex].layoutMaps[layoutIndex];

			// handle difference in number of components (a new component type has been implemented)
			if (numberOfComponentTypes < NUMBER_OF_COMPONENT_TYPES) 
				for (componentTypeIndex = componentTypeIndex; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) {
					(*componentLists)[componentTypeIndex] = malloc(sizeof(void*));

					(*layouts)[layoutIndex].componentListsPointers[componentTypeIndex] = (*componentLists)[componentTypeIndex];
					(*layouts)[layoutIndex].componentMaps[componentTypeIndex].start = 0;
					(*layouts)[layoutIndex].componentMaps[componentTypeIndex].end = 0;
				}
		}
	}

	for (int i = 0; i < numberOfComponentTypes; i++) {
		free(serialisationMapFragments[i].layoutMaps);
	}
	free(serialisationMapFragments);
}

void ECS_load(Layout** layoutsPtr, void*** componentListsPtr, char path[255], GameResources* resources) {
	// open file
	FILE* f;
	fopen_s(&f, path, "rb");
	if (NULL == f) exit(1);

	// get number of component types in save file
	int numberOfComponentTypes;
	fread(&numberOfComponentTypes, sizeof(int), 1, f);
	if (numberOfComponentTypes != NUMBER_OF_COMPONENT_TYPES) exit(1);
	void** componentLists = (void**)malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(void*));

	// get number of layouts in save file
	int numberOfLayouts;
	fread(&numberOfLayouts, sizeof(int), 1, f);
	Layout* layouts = (Layout*)calloc(numberOfLayouts, sizeof(Layout));
	if (NULL == layouts) exit(1);
	for (int i = 0; i < numberOfLayouts; i++)
		layouts[i].componentListsPointers = calloc(NUMBER_OF_COMPONENT_TYPES, sizeof(void*));

	// load serialisation fragments from file
	SerialisationMapFragment* serialisationMapFragments;
	serialisationMapFragments = (SerialisationMapFragment*)calloc(numberOfComponentTypes, sizeof(SerialisationMapFragment));
	if (NULL == serialisationMapFragments) exit(1);
	for (int componentTypeIndex = 0; componentTypeIndex < numberOfComponentTypes; componentTypeIndex++) {
		serialisationMapFragments[componentTypeIndex].layoutMaps = (LayoutMap*)calloc(numberOfLayouts, sizeof(LayoutMap));
		if (NULL == serialisationMapFragments[componentTypeIndex].layoutMaps) exit(1);

		fread(&serialisationMapFragments[componentTypeIndex].componentSize, sizeof(size_t), 1, f);
		fread(&serialisationMapFragments[componentTypeIndex].componentType, sizeof(int), 1, f);
		fread(serialisationMapFragments[componentTypeIndex].layoutMaps, sizeof(LayoutMap), numberOfLayouts, f);
		fread(&serialisationMapFragments[componentTypeIndex].total_components, sizeof(int), 1, f);
		fread(&serialisationMapFragments[componentTypeIndex].total_layouts, sizeof(int), 1, f);
	}

	if (NULL == componentLists) exit(1);
	for (int componentTypeIndex = 0; componentTypeIndex < numberOfComponentTypes; componentTypeIndex++) {
		int total_components = serialisationMapFragments[componentTypeIndex].total_components;
		size_t componentSize = serialisationMapFragments[componentTypeIndex].componentSize;
		if (componentSize != ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL)) exit(1);

		componentLists[componentTypeIndex] = (void*)malloc(total_components * componentSize);
		if (NULL == componentLists[componentTypeIndex]) exit(1);

		fread(componentLists[componentTypeIndex], componentSize, total_components, f);

		// some components contain pointers or have values which should not be the same as before saving, so these have to be set to their desired values. 
		// sadly I couldn't think of a solution where this isn't hardcoded. :(
		if (componentTypeIndex == TILE) for (int i = 0; i < serialisationMapFragments[componentTypeIndex].total_components; i++)
			((Tile*)componentLists[componentTypeIndex])[i].tilemap = &resources->tilemap;
		if (componentTypeIndex == SPRITE) for (int i = 0; i < serialisationMapFragments[componentTypeIndex].total_components; i++)
			((Sprite*)componentLists[componentTypeIndex])[i].tilemap = &resources->tilemap;
		//if (componentTypeIndex == EDITOR) for (int i = 0; i < serialisationMapFragments[componentTypeIndex].total_components; i++)
		//	((Editor*)componentLists[componentTypeIndex])[i].copied = false;
		if (componentTypeIndex == ANIMATION) for (int i = 0; i < serialisationMapFragments[componentTypeIndex].total_components; i++) {
			((Animation*)componentLists[componentTypeIndex])[i].currentFrame = 0;
			((Animation*)componentLists[componentTypeIndex])[i].lastUpdateTime = SDL_GetTicks();
		}
	}
	fclose(f);

	ECS_deserialise(&layouts, numberOfLayouts, &componentLists, serialisationMapFragments, numberOfComponentTypes);

	*layoutsPtr = layouts;
	*componentListsPtr = componentLists;

	//Position test[61];
	//for (int i = 0; i < serialisationMapFragments[POSITION].total_components; i++)
	//	test[i] = ((Position*)layouts->componentListsPointers[POSITION])[i];
}

SerialisationMapFragment* ECS_serialise(Layout* layouts, int numberOfLayouts) {
	SerialisationMapFragment* serialisationMapFragments;
	serialisationMapFragments = (SerialisationMapFragment*)malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(SerialisationMapFragment));
	if (NULL == serialisationMapFragments) exit(1);

	for (int componentTypeIndex = 0; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) {
		serialisationMapFragments[componentTypeIndex].componentSize = ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL);
		serialisationMapFragments[componentTypeIndex].total_layouts = numberOfLayouts;
		serialisationMapFragments[componentTypeIndex].total_components = 0;
		serialisationMapFragments[componentTypeIndex].componentType = componentTypeIndex;

		for (int layoutIndex = 0; layoutIndex < numberOfLayouts; layoutIndex++) {
			serialisationMapFragments[componentTypeIndex].total_components += ECS_getNumberOfComponents(componentTypeIndex, layouts[layoutIndex]);

			serialisationMapFragments[componentTypeIndex].layoutMaps = (LayoutMap*)malloc(numberOfLayouts * sizeof(LayoutMap));
			if (NULL == serialisationMapFragments[componentTypeIndex].layoutMaps) exit(1);
			serialisationMapFragments[componentTypeIndex].layoutMaps[layoutIndex] = layouts[layoutIndex].componentMaps[componentTypeIndex];
		}
	}

	return serialisationMapFragments;
}

void* ECS_getComponent(ComponentType componentType, Layout currentLayout, int entity_ID) {
	// get the number of components of a certain type in a given layout
	int numOfComponents = ECS_getNumberOfComponents(componentType, currentLayout);
	// get the components of the given type
	void* components = ECS_getComponentList(componentType, currentLayout);

	Position test[100];
	for (int i = 0; i < numOfComponents; i++)
		test[i] = ((Position*)components)[i];

	for (int i = 0; i < numOfComponents; i++) {
		switch (componentType)
		{
		case POSITION: if (((Position*)components)[i].ENTITY_ID == entity_ID) return &((Position*)components)[i]; break;
		case SPRITE: if (((Sprite*)components)[i].ENTITY_ID == entity_ID) return &((Sprite*)components)[i]; break;
		case TILE: if (((Tile*)components)[i].ENTITY_ID == entity_ID) return &((Tile*)components)[i]; break;
		case EDITOR: if (((Editor*)components)[i].ENTITY_ID == entity_ID) return &((Editor*)components)[i]; break;
		case ANIMATION: if (((Animation*)components)[i].ENTITY_ID == entity_ID) return &((Animation*)components)[i]; break;
		case TEXT: if (((Text*)components)[i].ENTITY_ID == entity_ID) return &((Text*)components)[i]; break;
		case COLLIDER: if (((Collider*)components)[i].ENTITY_ID == entity_ID) return &((Collider*)components)[i]; break;
		case PHYSICS_BODY: if (((PhysicsBody*)components)[i].ENTITY_ID == entity_ID) return &((PhysicsBody*)components)[i]; break;
		case COLLISION_BOX: if (((CollisionBox*)components)[i].ENTITY_ID == entity_ID) return &((CollisionBox*)components)[i]; break;
		default:
			break;
		}
	}
	return NULL;
}

size_t ECS_getSizeAndTypeOfComponent(ComponentType componentType, char* componentTypePtr) {
	switch (componentType)
	{
	case POSITION: if (componentTypePtr != NULL) strcpy(componentTypePtr, "position"); return sizeof(Position); break;
	case SPRITE: if (componentTypePtr != NULL) strcpy(componentTypePtr, "sprite"); return sizeof(Sprite); break;
	case TILE: if (componentTypePtr != NULL) strcpy(componentTypePtr, "tile"); return sizeof(Tile); break;
	case EDITOR: if (componentTypePtr != NULL) strcpy(componentTypePtr, "editor"); return sizeof(Editor); break;
	case ANIMATION: if (componentTypePtr != NULL) strcpy(componentTypePtr, "animation"); return sizeof(Animation); break;
	case TEXT: if (componentTypePtr != NULL) strcpy(componentTypePtr, "text"); return sizeof(Text); break;
	case COLLIDER: if (componentTypePtr != NULL) strcpy(componentTypePtr, "collider"); return sizeof(Collider); break;
	case PHYSICS_BODY: if (componentTypePtr != NULL) strcpy(componentTypePtr, "physics_body"); return sizeof(PhysicsBody); break;
	case COLLISION_BOX: if (componentTypePtr != NULL) strcpy(componentTypePtr, "collision_box"); return sizeof(CollisionBox); break;
	default:
		return 0; break;
	}
}

void* ECS_getComponentList(ComponentType componentType, Layout currentLayout) {
	return (Position*)currentLayout.componentListsPointers[componentType] + currentLayout.componentMaps[componentType].start * ECS_getSizeAndTypeOfComponent(componentType, NULL);
}

void* ECS_getNthComponent(ComponentType componentType, Layout* currentLayout, int index) {
	return (char*)ECS_getComponentList(componentType, *currentLayout) + index * ECS_getSizeAndTypeOfComponent(componentType, NULL);
}

int ECS_getNumberOfComponents(ComponentType componentType, Layout currentLayout) {
	return currentLayout.componentMaps[componentType].end - currentLayout.componentMaps[componentType].start;
}

char** ECS_getEntity(Layout currentLayout, int enitity_ID) {
	char** components = (char**)malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(char*));
	if (NULL == components) exit(1);

	for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++) {
		components[i] = ECS_getComponent(i, currentLayout, enitity_ID);
	}

	return components;
}
