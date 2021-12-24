#ifndef ECS_H
#define ECS_H

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
#include "EntityRenderer.h"
#include "MovementController.h"
#include "Interactable.h"
#include "Health.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


// debug
SDL_Renderer* ECS__debug_renderer;
// debug

/**
A struct containing parallax data, and tells the renderer which order should the entities be rendered in. 
*/
typedef struct Layer {
	int zIndex;						///< Represents the z coordinate of the layer's child entities. The higher it is, the later the element will be rendered. 
	/**
	A 2d vector which tells the renderer the amount it should transform an element by the cameras vector.
	If 0, the entity will be stationary if 1, the entity moves to the opposite direction from the camera.
	*/
	Vec2 parallax;
} Layer;

#define NUMBER_OF_COMPONENT_TYPES 13
/**
This is an integer which represents a certain type of component. 
*/
typedef enum ComponentType {
	POSITION = 0,
	EDITOR = 1,
	SPRITE = 2,
	TILE = 3,
	TEXT = 4,
	ANIMATION = 5,
	COLLIDER = 6,
	COLLISION_BOX = 7,
	PHYSICS_BODY = 8,
	ENTITY_RENDERER = 9,
	MOVEMENT_CONTROLLER = 10,
	INTERACTABLE = 11,
	HEALTH = 12
} ComponentType;

//typedef struct BluePrintFunctions {
//	BluePrintFunction functions[NUMBER_OF_COMPONENT_TYPES];
//} BluePrintFunctions;

typedef struct LayoutMap {
	int start;						///< The index of the first element of the list of a certain type of components, which belongs to this layout.
	int end;						///< The index after the last element of the list of a certain type of components, which belongs to this layout.
} LayoutMap;

typedef struct Layout {
	char LAYOUT_NAME[255];			///< Name of the layout. 
	Vec2 camera;					///< A 2d vector, representing the camera. 
	Layer layers[16];				///< 16 member array containing the layout's layers. 

	LayoutMap* componentMaps;		///< A layout map, which contains data of the layout's child enities' location in their list. 
	void** componentListsPointers;	///< An array of pointers. Each pointer points to a different component type's list. 
} Layout;

/**
A struct containing data about a certain type of component. This struct is used in saving or loading game data. 
*/
typedef struct SerialisationMapFragment {
	ComponentType componentType;	///< An integer, representing the component's type. (eg.: Position(0), Sprite(2), ...)
	int total_components;			///< The number of components of the serialisation map fragment's components' type.
	size_t componentSize;			///< The size of a type of component which this map represents. (eg.: sizeof(Position))

	int total_layouts;				///< The number of layouts.
	LayoutMap* layoutMaps;			///< An array of serialised layouts.
} SerialisationMapFragment;

/**
* Saves the game data a file at a specified path. 
* @param layouts List of layouts. 
* @param numberOfLayouts The number of layouts in the game object. 
* @param playerName The name of the current player. This will be the save-file's name (./saves/<playerName>.data)
*/
void ECS_save(Layout* layouts, int numberOfLayouts, char playerName[255]);

/**
* Creates layout structure from serialisationMapFragments. 
* @param layouts A pointer to the list of layouts where the data should be writen.
* @param numberOfLayouts The number of layouts in the save-file. 
* @param componentLists A pointer to the list of pointers which point to their respective component type. 
* @param serialisationMapFragments A list of serialisation map fragments. This is used, when constructing layout infrastructure.
* @param numberOfComponentTypes The number of component types found in the save-file. 
*/
void ECS_deserialise(Layout** layouts, int numberOfLayouts, void*** componentLists, SerialisationMapFragment* serialisationMapFragments, int numberOfComponentTypes);

/**
* Load game data from file.
* @param layoutsPtr A pointer to the layouts list in the game object. This is where the layouts will be constructed from the instructions stored in the save file.
* @param componentListsPtr A pointer the the component lists in the game object. This is where the components will be loaded to from save file.
* @param path The path of the file. (Relative. eg.: "./saves/original.data")
* @param resources The resources which should be used when constructing components. (Eg.: tilemaps, fonts)
*/
void ECS_load(Layout** layoutsPtr, int* numberOfLayouts, char playerName[255], GameResources* resources);

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
* Return the nth element of a given type in the current layout. 
* @param componentType The type of the components.
* @param currentLayout The layout where we want to get the component from. 
* @param index The index of the component in the list. 
* @returns A pointer to the component. 
*/
void* ECS_getNthComponent(ComponentType componentType, Layout* currentLayout, int index);

/**
* Returns the number of a given component type in a given layout. 
* @param componentType The type of the components.
* @param currentLayout The layout where we want to get the component from. 
* @returns The number of components on the layout. 
*/
int ECS_getNumberOfComponents(ComponentType componentType, Layout currentLayout);

/** 
* Returns the first unused ID in a given layout. 
* @param currentLayout The layout where we are looking for the free ID. 
* @returns The first unused ID. 
*/
int ECS_getFreeID(Layout* currentLayout);

/**
* Returns a list of components which belonge to an entity with a given ID. 
* @param currentLayout The layout where we want to collect the component data from. 
* @param entity_ID ID of the entity which we are looking for. 
* @returns A dynamic list of pointers. Each of these pointers point to a diferent type of component. 
*/
void** ECS_getEntity(Layout currentLayout, int enitity_ID);

/**
 * Frees the list of pointers allocated by the ECS_getEntity function. 
 * @param entityComponents A pointer to the allocated memory that is to be freed. 
 */
void ECS_freeEntity(void** entityComponents);

/**
 * Returns a pointer to a component's ID. 
 * @param componentType Type of the component. 
 * @param component A pointer to the component. 
 * @returns A pointer to the component's ID.
 */
int* ECS_getEntityIDPtr(ComponentType componentType, void* component);

/**
 * Creates a new layout. 
 * @param layoutsPtr A pointer to the list of layouts in the game object. 
 * @param numberOfLayouts A pointer to the integer in the game object, that stores the number of layouts. 
 * @param layoutName Name of the layout to be created.
 */
void ECS_createLayout(Layout** layoutsPtr, int* numberOfLayouts, char layoutName[255]);

/**
 * Deletes a given component.
 * @param component Type Type of the component. 
 * @param layouts The list of layouts stored in the game object. 
 * @param numberOfLayouts The number of layouts. 
 * @param layoutName Name of the layout where the component should be deleted from. 
 * @param entity_ID ID of the component to be deleted. 
 */
void ECS_deleteComponent(ComponentType componentType, Layout* layouts, int numberOfLayouts, char* layoutName, int entity_ID);

/**
 * Checks whether a given layout's name matches a given string.
 * @param currentLayout A pointer to the layout.
 * @param name The string which the layout's name should be compared with. 
 * @return Logic true if the strings match, false if they don't. 
 */
bool ECS_layoutHasName(Layout* currentLayout, char* name);

/**
 * Returns a pointer to a layout with a given name.
 * @param layouts The list of layouts.
 * @param numberOfLayouts The number of layouts.
 * @param LAYOUT_NAME The name of the layout we are looking for. 
 * @return A pointer to the layout. 
 */
Layout* ECS_getLayout(Layout* layouts, int numberOfLayouts, char* LAYOUT_NAME);

/**
 * Returns the index of the layout whith a given name.
 * @param layouts The list of layouts.
 * @param numberOfLayouts The number of layouts.
 * @param LAYOUT_NAME The name of the layout we are looking for.
 * @returns The index. 
 */
int ECS_getLayoutIndex(Layout* layouts, int numberOfLayouts, char* LAYOUT_NAME);

/**
 * Frees the data of all layouts.
 * @param layouts The list of layouts.
 * @param numberOfLayouts The number of layouts.
 */
void ECS_freeData(Layout* layouts, int numberOfLayouts);

/**
 * Creates a a new component of a given type.
 * @param componentType Type of the component.
 * @param layouts The list of layouts.
 * @param numberOfLayouts The number of layouts.
 * @param layoutName The name of the layout where the component should be created. 
 * @param entity_ID The component's parent entity's ID.
 * @returns A pointer to the created component. 
 */
void* ECS_createComponent(ComponentType componentType, Layout* layouts, int numberOfLayouts, char* layoutName, int enitity_ID);

/**
 * Saves game and loads another player's game.
 */
void ECS_switchToPlayer(char* currentPlayer, char* player, Layout** layoutsPtr, int* numberOfLayouts, Layout** currentLayoutPtr, GameResources* resources);

typedef struct BluePrintMember {
	char name[32];
	size_t size;
	void* ptr;
	char format[32];
} BluePrintMember;

typedef struct BluePrint {
	ComponentType componentType;
	int numberOfMembers;
	BluePrintMember* members;
} BluePrint;

void ECS_bluePringAddMember(BluePrint* bluePrint, BluePrintMember newMember);
BluePrint ECS_getBluePrint(ComponentType componentType, void* componentPtr);
BluePrint(*ECS_BluePrintFunctions[NUMBER_OF_COMPONENT_TYPES]) (ComponentType componentType, void* component);

void ECS_restartGame(char* currentPlayer, Layout** layoutsPtr, int* numberOfLayouts, Layout** currentLayoutPtr, GameResources* resources);

// TODO: Decrease the number of parameters passed to functions which have to create of delete components. 
// maybe the struct bellow should be passed to functions as opposed to a list of it's members: (layouts and layoutname and numberoflayouts and so on).
// (should check whether it would be easy to implement)
// Also maybe a pointer to this object should be globaly accessable. (not sure if allowed in assignment)
// -----------------
//typedef struct ECS {
//	char playerName[256];
//
//	Layout* currentLayout;
//	Layout* layouts;
//	int numberOfLayouts;
//
//	char resources; // not char
//} ECS;
// -----------------

#endif // !ECS_H

