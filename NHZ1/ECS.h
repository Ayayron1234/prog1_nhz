#pragma once
#include <stdbool.h>
#include <string.h>
#include "misc.h"
#include "Position.h"
#include "Sprite.h"
#include "Editor.h"
#include "Animation.h"
#include "Tile.h"
#include "Text.h"
#include "CollisionBox.h"
#include "Collider.h"
#include "PhysicsBody.h"
//#include "debugmalloc.h"

typedef struct Layer {
	int zIndex;
	Vec2 parallax;
} Layer;

#define NUMBER_OF_COMPONENT_TYPES 9
typedef enum ComponentType {
	POSITION = 0,
	EDITOR = 1,
	SPRITE = 2,
	TILE = 3,
	TEXT = 4,
	ANIMATION = 5,
	COLLIDER = 6,
	COLLISION_BOX = 7,
	PHYSICS_BODY = 8
} ComponentType;

typedef struct LayoutMap {
	int start;
	int end;
} LayoutMap;

typedef struct Layout {
	char LAYOUT_NAME[255];
	Vec2 camera;
	Layer layers;

	LayoutMap* componentMaps;
	void** componentListsPointers;
} Layout;

typedef struct SerialisationMapFragment {
	ComponentType componentType; // eg.: Position, Sprite, ...
	int total_components;        // number of components of a certain type
	size_t componentSize;        // sizeof(ComponentTypeX)

	int total_layouts;           // number of layouts
	LayoutMap* layoutMaps;       // array of serialised layouts
} SerialisationMapFragment;

/**
* Creates layout structure from serialisationMapFragments. 
*/
void ECS_deserialise(Layout** layouts, int numberOfLayouts, void*** componentLists, SerialisationMapFragment* serialisationMapFragments, int numberOfComponentTypes);

/**
* Load game data from file.
* @param layoutsPtr A pointer to the layouts list in the game object. This is where the layouts will be constructed from the instructions stored in the save file.
* @param componentListsPtr A pointer the the component lists in the game object. This is where the components will be loaded to from save file.
* @param path The path of the file. (Relative. eg.: "./saves/original.data")
* @param resources The resources which should be used when constructing components. (Eg.: tilemaps, fonts)
*/
void ECS_load(Layout** layoutsPtr, void*** componentListsPtr, char path[255], GameResources* resources);

/**
* Creates serialisation map fragments of the layouts. (More precise explanation in the prog1_nzh_devdocs)
* @param layouts list of layouts from which the serialisationMapFragments should be constructed. 
* @param numberOfLayouts number of layouts.
* @returns a list of SerialisationMapFragments. 
*/
SerialisationMapFragment* ECS_serialise(Layout* layouts, int numberOfLayouts);

/**
* Returns the size and name of a given component type.
* @param componentType Type of component which's data should be recieved.
* @param componentTypePtr a pointer to a string where the component type should be stored. 
* @returns size of the given component
*/
size_t ECS_getSizeAndTypeOfComponent(ComponentType componentType, char* componentTypePtr);

/**
* Get a certain type of component from a layout. 
* @param componentType The type of the component. 
* @param currentLayout The parent layout of the component. 
* @param entity_ID The id of the component. 
* @returns A void pointer to the component. 
*/
void* ECS_getComponent(ComponentType componentType, Layout currentLayout, int entity_ID);

/**
* Get list of components of a certain type belonging to a given layout. Works in combination with ECS_getNumberOfComponents(...)
* @param componentType The type of the components.
* @param currentLayout The layout to which the components should belong to. 
* @returns A void pointer to the beginning of the components belonging to the layout. 
*/
void* ECS_getComponentList(ComponentType componentType, Layout currentLayout);

/**
* 
*/
void* ECS_getNthComponent(ComponentType componentType, Layout* currentLayout, int index);
/**
*
*/
int ECS_getNumberOfComponents(ComponentType componentType, Layout currentLayout);

/**
*
*/
void** ECS_getEntity(Layout currentLayout, int enitity_ID);
void ECS_freeEntity(void** entityComponents);

typedef struct ComponentLists {
	int total_positionComponents;
	Position *positionComponents;
	int total_spriteComponents;
	Sprite *spriteComponents;
	int total_editorComponents;
	Editor* editorComponents;
	int total_animationComponents;
	Animation* animationComponents;
	int total_tileComponents;
	Tile* tileComponents;
	int total_textComponents;
	Text* textComponents;
	int total_collisionBoxComponents;
	CollisionBox* collisionBoxComponents;
	int total_colliderComponents;
	Collider* colliderComponents;
	int total_physicsBodyComponents;
	PhysicsBody* physicsBodyComponents;
} ComponentLists;

// temp --------------------------------------------------------
Position* ECS_getPositionComponent(ComponentLists* components, int entityID);
Sprite* ECS_getSpriteComponent(ComponentLists* components, int entityID);
Editor* ECS_getEditorComponent(ComponentLists* components, int entityID);
Animation* ECS_getAnimationComponent(ComponentLists* components, int entityID);
Tile* ECS_getTileComponent(ComponentLists* components, int entityID);
Text* ECS_getTextComponent(ComponentLists* components, int entityID);
CollisionBox* ECS_getCollisionBoxComponent(ComponentLists* components, int entityID);
Collider* ECS_getColliderComponent(ComponentLists* components, int entityID);
PhysicsBody* ECS_getPhysicsBodyComponent(ComponentLists* components, int entityID);
// temp --------------------------------------------------------

ComponentLists ECS_init(int maxNumberOfComponents, bool doDeserialisation, char saveDirectory[255], Tilemap *tilemap);

int ECS_createEntity(ComponentLists* components, int maxNumberOfComponents);
void ECS_deleteEntity(ComponentLists* components, int entityID);
void ECS_printEntityData(ComponentLists* components, int entityID);
