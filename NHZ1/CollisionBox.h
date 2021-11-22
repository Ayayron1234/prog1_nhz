#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "SDL.h"
#include "Math.h"

/**
Stores the size of the entity. This components is mainly, but not exclusively used for collision and click detection. 
*/
typedef struct CollisionBox {
	int ENTITY_ID;

	Vec2 size;
} CollisionBox;

///**
//* Create CollisionBox component in a given ComponentLists.
//* @param collisionBoxes The array in which the component should be created. 
//* @param entityID The ID of the component's parent entity.
//* @param total_collisionBoxComponents A pointer to the variable which stores the number of components of this kind in it's parent layout.
//* @param size Size of the entity's collision box.
//*/
//void CollisionBox_init(CollisionBox* collisionBoxes, int entityID, int* total_collisionBoxComponents, Vec2 size);
//
///**
//* Deletes a given entity's CollisionBox component.
//* @param The array from which the component should be deleted. 
//* @param entityID The ID of the component's parent entity.
//* @param total_collisionBoxComponents A pointer to the variable which stores the number of components of this kind in it's parent layout.
//*/
//void CollisionBox_delete(CollisionBox* collisionBoxes, int entityID, int* total_collisionBoxComponents);
//
//// temp
//void CollisionBox_deserialise(CollisionBox* collisionBoxes, int* total_collisionBoxComponents, int maxNumberOfComponents, char path[255]);
//void CollisionBox_serialise(CollisionBox* collisionBoxes, int maxNumberOfComponents, char path[255]);
