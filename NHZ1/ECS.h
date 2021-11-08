#pragma once
#include <stdbool.h>
#include <string.h>
#include "Position.h"
#include "Sprite.h"
#include "Editor.h"
#include "Animation.h"
#include "Tile.h"
#include "Text.h"
#include "CollisionBox.h"
#include "Collider.h"
#include "PhysicsBody.h"


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

ComponentLists ECS_init(int maxNumberOfComponents, bool doDeserialisation, char saveDirectory[255], Tilemap *tilemap);

int ECS_createEntity(ComponentLists* components, int maxNumberOfComponents);
void ECS_deleteEntity(ComponentLists* components, int entityID);
void ECS_printEntityData(ComponentLists* components, int entityID);

Position* ECS_getPositionComponent(ComponentLists* components, int entityID);
Sprite* ECS_getSpriteComponent(ComponentLists* components, int entityID);
Editor* ECS_getEditorComponent(ComponentLists* components, int entityID);
Animation* ECS_getAnimationComponent(ComponentLists* components, int entityID);
Tile* ECS_getTileComponent(ComponentLists* components, int entityID);
Text* ECS_getTextComponent(ComponentLists* components, int entityID);
CollisionBox* ECS_getCollisionBoxComponent(ComponentLists* components, int entityID);
Collider* ECS_getColliderComponent(ComponentLists* components, int entityID);
PhysicsBody* ECS_getPhysicsBodyComponent(ComponentLists* components, int entityID);
