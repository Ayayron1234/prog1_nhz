#pragma warning(disable : 4996)
#include "ECS.h"

void ECS_printEntityData(Layout* currentLayout, int entityID) {
	void** entity = ECS_getEntity(*currentLayout, entityID);
	
	printf("\n\nEntity #%d", entityID);

	Position* pos = entity[POSITION];
	if (pos != NULL) 
		printf("\n   >Position\n     .value=Vec2(%g, %g)", pos->value.x, pos->value.y);
	CollisionBox* collisionBox = entity[COLLISION_BOX];
	if (collisionBox != NULL)
		printf("\n   >CollisionBox\n     .size=Vec2(%g, %g)", collisionBox->size.x, collisionBox->size.y);
	Sprite* sprite = entity[SPRITE];
	if (sprite != NULL) 
		printf("\n   >Sprite\n     .tilePosition=Vec2Int(%d, %d)", sprite->tilePosition.x, sprite->tilePosition.y);
	Animation* animation = entity[ANIMATION];
	if (animation != NULL)
		printf("\n   >Animation\n     .tilePosition=Vec2Int(%d, %d)\n     .frameCount=%d\n     .animationSpeed=%g",
			animation->tilePosition.x, animation->tilePosition.y, animation->frameCount, animation->animationSpeed);
	Tile* tile = entity[TILE];
	if (tile != NULL)
		printf("\n   >Tile\n     .tilePosition=Vec2Int(%d, %d)\n     .size=Vec2Int(%d, %d)",
			tile->tilePosition.x, tile->tilePosition.y, tile->size.x, tile->size.y);
	Text* text = entity[TEXT];
	if (text != NULL)
		printf("\n   >Text\n     .value=%s\n     .textBoxSize=Vec2Int(%d, %d)\n     .fontColor=RGB(%d, %d, %d)",
			text->value, text->textBoxSize.x, text->textBoxSize.y, text->fontColor.r, text->fontColor.g, text->fontColor.b);
	Collider* collider = entity[COLLIDER];
	if (collider != NULL)
		printf("\n   >Collider\n     .type=%s", (collider->type == DYNAMIC) ? "DYNAMIC" : "STATIC");
	PhysicsBody* physicsBody = entity[PHYSICS_BODY];
	if (physicsBody != NULL)
		printf("\n   >PhysicsBody\n     .mass=%g\n     .gravitationalAcceleration=Vec2(%g, %g)\n     .velocity=Vec2(%g, %g)\n     .acceleration=Vec2(%g, %g)", 
			physicsBody->mass, physicsBody->gravitationalAcceleration.x, physicsBody->gravitationalAcceleration.y, physicsBody->velocity.x, physicsBody->velocity.y,
			physicsBody->acceleration.x, physicsBody->acceleration.y);
	EntityRenderer* renderer = entity[ENTITY_RENDERER];
	if (renderer != NULL)
		printf("\n   >EnitityRenderer\n     .layerIndex=%d\n     .filter=%s", renderer->layerIndex, 
			  (renderer->filter == 0) ? "RENDER_FILTER_ALLWAYS" : (renderer->filter == 1) ? "RENDER_FILTER_EDIT" : "RENDER_FILTER_GAME");
	MovementController* movementController = entity[MOVEMENT_CONTROLLER];
	if (movementController != NULL)
		printf("\n   >MovementController\n     .movementStyle=%s\n     .collisionNormal=Vec2(%g, %g)\n     .acceleration=%g\n     .deceleration=%g\n     .maxSpeed=%g\n"
			   "     .maxFallSpeed=%g\n     .gravity=%g\n     .jumpHeight=%g\n     .timeToApex=%g\n     .doubleJump=%s", (movementController->movementStyle == 0) ? "PLATFORMER" : "GOOMBA",
			   movementController->collisionNormal.x, movementController->collisionNormal.y, movementController->acceleration, movementController->deceleration, movementController->maxSpeed,
			   movementController->maxFallSpeed, movementController->gravity, movementController->jumpHeight, movementController->timeToApex, (movementController->doubleJump) ? "true" : "false");
	Interactable* interactable = entity[INTERACTABLE];
	if (interactable != NULL) {
		printf("\n   >Interactable\n     .type=");
		switch (interactable->type)
		{
		case BUTTON: printf("BUTTON"); break;
		case TEXTBOX: printf("TEXTBOX"); break;
		case LUCKY_BLOCK: printf("LUCKY_BLOCK"); break;
		case ENEMY: printf("ENEMY"); break;
		case HEALTH_PICKUP: printf("HEALTH_PICKUP"); break;
		case COIN: printf("COIN"); break;
		case FINISH: printf("FINISH"); break;
		default:
			break;
		}
		printf("\n     .hasFocus=%s\n     .nextInFocus=%d", (interactable->hasFocus) ? "true" : "false", interactable->nextInFocus);
	}
	Editor* editor = entity[EDITOR];
	if (editor != NULL)
		printf("\n   >Editor\n     .copied=%s\n", (editor->copied) ? "true": "false");

	ECS_freeEntity(entity);
}

void ECS_deserialise(Layout** layouts, int numberOfLayouts, void*** componentLists, SerialisationMapFragment* serialisationMapFragments, int numberOfComponentTypes) {
	for (int layoutIndex = 0; layoutIndex < numberOfLayouts; layoutIndex++) {
		(*layouts)[layoutIndex].componentMaps = (LayoutMap*)calloc(NUMBER_OF_COMPONENT_TYPES, sizeof(LayoutMap));

		if (NULL == (*layouts)[layoutIndex].componentMaps) exit(1);

		for (int componentTypeIndex = 0; componentTypeIndex < numberOfComponentTypes; componentTypeIndex++) {
			(*layouts)[layoutIndex].componentListsPointers[componentTypeIndex] = (*componentLists)[componentTypeIndex];
			(*layouts)[layoutIndex].componentMaps[componentTypeIndex] = serialisationMapFragments[componentTypeIndex].layoutMaps[layoutIndex];

			// handle difference in number of components (a new component type has been implemented)
			if (numberOfComponentTypes < NUMBER_OF_COMPONENT_TYPES && componentTypeIndex >= numberOfComponentTypes)
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

	//for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++)
	//	free(componentLists[i]);
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

		serialisationMapFragments[componentTypeIndex].layoutMaps = (LayoutMap*)malloc(numberOfLayouts * sizeof(LayoutMap));
		if (NULL == serialisationMapFragments[componentTypeIndex].layoutMaps) exit(1);
		for (int layoutIndex = 0; layoutIndex < numberOfLayouts; layoutIndex++) {
			serialisationMapFragments[componentTypeIndex].total_components += ECS_getNumberOfComponents(componentTypeIndex, layouts[layoutIndex]);

			serialisationMapFragments[componentTypeIndex].layoutMaps[layoutIndex] = layouts[layoutIndex].componentMaps[componentTypeIndex];
		}
	}

	return serialisationMapFragments;
}

void ECS_save(Layout* layouts, int numberOfLayouts, char playerName[255]) {
	// create serialisation map fragments
	SerialisationMapFragment* serialisationMapFragments = ECS_serialise(layouts, numberOfLayouts);

	// construct file path
	char path[255];
	sprintf(path, "./saves/%s.data", playerName);

	// empty file
	FILE* f = fopen(path, "wb");
	if (NULL == f) exit(1);

	// write number of component types to the file
	int numberOfComponents = NUMBER_OF_COMPONENT_TYPES;
	fwrite(&numberOfComponents, sizeof(int), 1, f);
	fclose(f);

	// open file in write mode
	f = fopen(path, "ab");
	if (NULL == f) exit(1);

	// write the number of layouts to the file
	fwrite(&numberOfLayouts, sizeof(int), 1, f);

	// write layout data to the file
	fwrite(layouts, sizeof(Layout), numberOfLayouts, f);

	// write serialisation map fragments to the file
	for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++) {
		fwrite(&serialisationMapFragments[i].componentSize, sizeof(size_t), 1, f);
		fwrite(&serialisationMapFragments[i].componentType, sizeof(int), 1, f);
		fwrite(serialisationMapFragments[i].layoutMaps, sizeof(LayoutMap), numberOfLayouts, f);
		fwrite(&serialisationMapFragments[i].total_components, sizeof(int), 1, f);
		fwrite(&serialisationMapFragments[i].total_layouts, sizeof(int), 1, f);
	}

	// write components data to the file
	// (layouts[0] is correct because it contains components of all layouts)
	for (int componentTypeIndex = 0; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) {
		fwrite(layouts[0].componentListsPointers[componentTypeIndex], ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL), serialisationMapFragments[componentTypeIndex].total_components, f);
	}

	fclose(f);
	for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++)
		free(serialisationMapFragments[i].layoutMaps);
	free(serialisationMapFragments);
}

void ECS_load(Layout** layoutsPtr, int* numberOfLayouts, char playerName[255], GameResources* resources) {
	// construct file path
	char path[255];
	sprintf(path, "./saves/%s.data", playerName);

	// open file
	FILE* f = fopen(path, "rb");
	if (NULL == f) exit(1);

	// get number of component types in save file
	int numberOfComponentTypes;
	fread(&numberOfComponentTypes, sizeof(int), 1, f);
	//if (numberOfComponentTypes != NUMBER_OF_COMPONENT_TYPES) exit(1);
	void** componentLists = (void**)malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(void*));

	// get number of layouts in save file
	fread(numberOfLayouts, sizeof(int), 1, f);
	Layout* layouts = (Layout*)calloc(*numberOfLayouts, sizeof(Layout));
	if (NULL == layouts) exit(1);

	// load layout data from file
	fread(layouts, sizeof(Layout), *numberOfLayouts, f);
	for (int i = 0; i < *numberOfLayouts; i++)
		layouts[i].componentListsPointers = calloc(NUMBER_OF_COMPONENT_TYPES, sizeof(void*));

	// load serialisation fragments from file
	SerialisationMapFragment* serialisationMapFragments;
	serialisationMapFragments = (SerialisationMapFragment*)calloc(numberOfComponentTypes, sizeof(SerialisationMapFragment));
	if (NULL == serialisationMapFragments) exit(1);
	for (int componentTypeIndex = 0; componentTypeIndex < numberOfComponentTypes; componentTypeIndex++) {
		serialisationMapFragments[componentTypeIndex].layoutMaps = (LayoutMap*)calloc(*numberOfLayouts, sizeof(LayoutMap));
		if (NULL == serialisationMapFragments[componentTypeIndex].layoutMaps) exit(1);

		fread(&serialisationMapFragments[componentTypeIndex].componentSize, sizeof(size_t), 1, f);
		fread(&serialisationMapFragments[componentTypeIndex].componentType, sizeof(int), 1, f);
		fread(serialisationMapFragments[componentTypeIndex].layoutMaps, sizeof(LayoutMap), *numberOfLayouts, f);
		fread(&serialisationMapFragments[componentTypeIndex].total_components, sizeof(int), 1, f);
		fread(&serialisationMapFragments[componentTypeIndex].total_layouts, sizeof(int), 1, f);
	}

	// load component lists
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
		if (componentTypeIndex == EDITOR) for (int i = 0; i < serialisationMapFragments[componentTypeIndex].total_components; i++)
			((Editor*)componentLists[componentTypeIndex])[i].copied = false;
		if (componentTypeIndex == ANIMATION) for (int i = 0; i < serialisationMapFragments[componentTypeIndex].total_components; i++) {
			((Animation*)componentLists[componentTypeIndex])[i].currentFrame = 0;
			((Animation*)componentLists[componentTypeIndex])[i].lastUpdateTime = SDL_GetTicks();
		}
		if (componentTypeIndex == INTERACTABLE) for (int i = 0; i < serialisationMapFragments[componentTypeIndex].total_components; i++) {
			((Interactable*)componentLists[componentTypeIndex])[i].interactionHead = NULL;
			((Interactable*)componentLists[componentTypeIndex])[i].latestInteraction = NULL;
		}
	}
	fclose(f);

	ECS_deserialise(&layouts, *numberOfLayouts, &componentLists, serialisationMapFragments, numberOfComponentTypes);
	free(componentLists);

	*layoutsPtr = layouts;
}

void ECS_freeData(Layout* layouts, int numberOfLayouts) {
	for (int componentTypeIndex = 0; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) 
		free(layouts[0].componentListsPointers[componentTypeIndex]);

	for (int layoutIndex = 0; layoutIndex < numberOfLayouts; layoutIndex++) {
		free(layouts[layoutIndex].componentListsPointers);
		free(layouts[layoutIndex].componentMaps);
	}

	free(layouts);
}

void* ECS_createComponent(ComponentType componentType, Layout* layouts, int numberOfLayouts, char* layoutName, int enitity_ID) {
	int layoutIndex = ECS_getLayoutIndex(layouts, numberOfLayouts, layoutName);

	// check whether a component with the same id allready exists in the layout
	void* componentWithSameID = ECS_getComponent(componentType, layouts[layoutIndex], enitity_ID);
	if (componentWithSameID != NULL) return componentWithSameID;

	// get list of components with the given type, and calculate the new component's position
	void* componentList = layouts[layoutIndex].componentListsPointers[componentType];
	int positionInList = layouts[layoutIndex].componentMaps[componentType].end;

	// get total number of components on all layouts. 
	int total_components = 0;
	for (int i = 0; i < numberOfLayouts; i++) 
		total_components += layouts[i].componentMaps[componentType].end - layouts[i].componentMaps[componentType].start;

	// allocate memory for a new list of components
	void* newComponentList = calloc((++total_components), ECS_getSizeAndTypeOfComponent(componentType, NULL));
	if (NULL == newComponentList) exit(1);

	// copy components from the old list to the new one before the index of the new component
	memcpy(newComponentList, componentList, positionInList * ECS_getSizeAndTypeOfComponent(componentType, NULL));
	// copy components from the old list to the new one after the index of the new component
	memcpy((char*)newComponentList + (positionInList + 1) * ECS_getSizeAndTypeOfComponent(componentType, NULL),
		(char*)componentList + positionInList * ECS_getSizeAndTypeOfComponent(componentType, NULL),
		(total_components - positionInList - 1) * ECS_getSizeAndTypeOfComponent(componentType, NULL));

	void* newComponent = (char*)newComponentList + positionInList * ECS_getSizeAndTypeOfComponent(componentType, NULL);

	// set entity ID
	*ECS_getEntityIDPtr(componentType, newComponent) = enitity_ID;

	// update layout list
	//free(layouts[0].componentListsPointers[componentType]);
	for (int i = 0; i < numberOfLayouts; i++) {
		layouts[i].componentListsPointers[componentType] = newComponentList;
		if (i == layoutIndex)
			layouts[i].componentMaps[componentType].end += 1;
		if (i > layoutIndex) {
			layouts[i].componentMaps[componentType].start++;
			layouts[i].componentMaps[componentType].end++;
		}
	}

	//free(componentList);
	return newComponent;
}

void ECS_deleteComponent(ComponentType componentType, Layout* layouts, int numberOfLayouts, char* layoutName, int entity_ID) {
	int targetLayoutIndex = ECS_getLayoutIndex(layouts, numberOfLayouts, layoutName);

	// get list of components with the given type, and calculate the new component's position
	void* componentList = layouts[targetLayoutIndex].componentListsPointers[componentType];
	int positionInList = -1;
	for (int i = 0; i < ECS_getNumberOfComponents(componentType, layouts[targetLayoutIndex]); i++)
	if (*ECS_getEntityIDPtr(componentType, ECS_getNthComponent(componentType, &layouts[targetLayoutIndex], i)) == entity_ID) {
		positionInList = i;
		break;
	}
	for (int i = 0; i < targetLayoutIndex; i++)
		positionInList += layouts[i].componentMaps[componentType].end;

	// get total number of components on all layouts. 
	int total_components = 0;
	for (int i = 0; i < numberOfLayouts; i++)
		total_components += layouts[i].componentMaps[componentType].end - layouts[i].componentMaps[componentType].start;

	// allocate memory for a new list of components
	void* newComponentList = calloc((--total_components), ECS_getSizeAndTypeOfComponent(componentType, NULL));
	if (NULL == newComponentList) exit(1);

	// copy components from the old list to the new one before the index of the new component
	memcpy(newComponentList, componentList, positionInList * ECS_getSizeAndTypeOfComponent(componentType, NULL));
	// copy components from the old list to the new one after the index of the new component
	memcpy((char*)newComponentList + positionInList * ECS_getSizeAndTypeOfComponent(componentType, NULL),
		   (char*)componentList + (positionInList + 1) * ECS_getSizeAndTypeOfComponent(componentType, NULL),
		   (total_components - positionInList) * ECS_getSizeAndTypeOfComponent(componentType, NULL));

	// update layout list
	//free(layouts[0].componentListsPointers[componentType]);
	for (int i = 0; i < numberOfLayouts; i++) {
		layouts[i].componentListsPointers[componentType] = newComponentList;
		if (i == targetLayoutIndex)
			layouts[i].componentMaps[componentType].end -= 1;
		if (i > targetLayoutIndex) {
			layouts[i].componentMaps[componentType].start--;
			layouts[i].componentMaps[componentType].end--;
		}
	}
}

void* ECS_getComponent(ComponentType componentType, Layout currentLayout, int entity_ID) {
	for (int i = 0; i < ECS_getNumberOfComponents(componentType, currentLayout); i++) {
		int id = *ECS_getEntityIDPtr(componentType, ECS_getNthComponent(componentType, &currentLayout, i));
		if (id == entity_ID) return ECS_getNthComponent(componentType, &currentLayout, i);
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
	case ENTITY_RENDERER: if (componentTypePtr != NULL) strcpy(componentTypePtr, "entity_renderer"); return sizeof(EntityRenderer); break;
	case MOVEMENT_CONTROLLER: if (componentTypePtr != NULL) strcpy(componentTypePtr, "movement_controller"); return sizeof(MovementController); break;
	case INTERACTABLE: if (componentTypePtr != NULL) strcpy(componentTypePtr, "interactable"); return sizeof(Interactable); break;
	default:
		return 0; break;
	}
}

void* ECS_getComponentList(ComponentType componentType, Layout currentLayout) {
	return (char*)currentLayout.componentListsPointers[componentType] + currentLayout.componentMaps[componentType].start * ECS_getSizeAndTypeOfComponent(componentType, NULL);
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

	for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++)
		components[i] = ECS_getComponent(i, currentLayout, enitity_ID);

	return components;
}

void ECS_freeEntity(void** entityComponents) {
	free(entityComponents);
}

int* ECS_getEntityIDPtr(ComponentType componentType, void* component) {
	if (NULL == component) return NULL;

	switch (componentType)
	{
	case POSITION: return &((Position*)component)->ENTITY_ID; break;
	case SPRITE: return &((Sprite*)component)->ENTITY_ID; break;
	case TILE: return &((Tile*)component)->ENTITY_ID; break;
	case EDITOR: return &((Editor*)component)->ENTITY_ID; break;
	case ANIMATION: return &((Animation*)component)->ENTITY_ID; break;
	case TEXT: return &((Text*)component)->ENTITY_ID; break;
	case COLLIDER: return &((Collider*)component)->ENTITY_ID; break;
	case PHYSICS_BODY: return &((PhysicsBody*)component)->ENTITY_ID; break;
	case COLLISION_BOX: return &((CollisionBox*)component)->ENTITY_ID; break;
	case ENTITY_RENDERER: return &((EntityRenderer*)component)->ENTITY_ID; break;
	case MOVEMENT_CONTROLLER: return &((MovementController*)component)->ENTITY_ID; break;
	case INTERACTABLE: return &((Interactable*)component)->ENTITY_ID; break;
	default: return NULL;  break;
	}
}

void ECS_createLayout(Layout** layoutsPtr, int* numberOfLayouts, char layoutName[255]) {
	Layout* newLayouts;
	newLayouts = calloc(*numberOfLayouts + 1, sizeof(Layout));
	if (NULL == newLayouts) exit(1);

	memcpy(newLayouts, *layoutsPtr, *numberOfLayouts * sizeof(Layout));

	strcpy(newLayouts[*numberOfLayouts].LAYOUT_NAME, layoutName);
	newLayouts[*numberOfLayouts].camera = (Vec2){ 0, 0 };

	newLayouts[*numberOfLayouts].componentListsPointers = malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(void*));
	if (NULL == newLayouts[*numberOfLayouts].componentListsPointers) exit(1);
	newLayouts[*numberOfLayouts].componentMaps = malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(LayoutMap));
	if (NULL == newLayouts[*numberOfLayouts].componentMaps) exit(1);
	for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++) {
		newLayouts[*numberOfLayouts].componentListsPointers[i] = newLayouts[0].componentListsPointers[i];

		newLayouts[*numberOfLayouts].componentMaps[i].start = newLayouts[*numberOfLayouts - 1].componentMaps[i].end;
		newLayouts[*numberOfLayouts].componentMaps[i].end = newLayouts[*numberOfLayouts - 1].componentMaps[i].end;
	}
	
	*layoutsPtr = newLayouts;
	*numberOfLayouts += 1;
}

bool ECS_layoutHasName(Layout* currentLayout, char* name) {
	return strcmp(&currentLayout->LAYOUT_NAME, name) == 0;
}

Layout* ECS_getLayout(Layout* layouts, int numberOfLayouts, char* LAYOUT_NAME) {
	for (int i = 0; i < numberOfLayouts; i++)
		if (ECS_layoutHasName(&layouts[i], LAYOUT_NAME)) return &layouts[i];
	return NULL;
}

int ECS_getLayoutIndex(Layout* layouts, int numberOfLayouts, char* LAYOUT_NAME) {
	for (int i = 0; i < numberOfLayouts; i++)
		if (ECS_layoutHasName(&layouts[i], LAYOUT_NAME)) return i;
	return -1;
}

int ECS_getFreeID(Layout* currentLayout) {
	int freeIDs[256];
	for (int i = 0; i < 256; i++)
		freeIDs[i] = i;

	for (int componentTypeIndex = 0; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) {
		for (int i = 0; i < ECS_getNumberOfComponents(componentTypeIndex, *currentLayout); i++) {
			void* component = ECS_getNthComponent(componentTypeIndex, currentLayout, i);

			freeIDs[*ECS_getEntityIDPtr(componentTypeIndex, component)] = 0;
		}
	}

	for (int i = 0; i < 256; i++)
		if (freeIDs[i] != 0) return i;
	return 0;
}

void ECS_switchToPlayer(char *currentPlayer, char *player, Layout** layoutsPtr, int* numberOfLayouts, Layout** currentLayoutPtr, GameResources* resources) {
	//char* nextPlayerName = (char*)malloc((strlen(player) + 1) * sizeof(char));
	//if (NULL == nextPlayerName) return;
	//strcpy(nextPlayerName, player);

	ECS_save(*layoutsPtr, *numberOfLayouts, currentPlayer);
	strcpy(currentPlayer, player);
	ECS_freeData(*layoutsPtr, *numberOfLayouts);
	ECS_load(layoutsPtr, numberOfLayouts, currentPlayer, resources);
	*currentLayoutPtr = layoutsPtr[0];
}
