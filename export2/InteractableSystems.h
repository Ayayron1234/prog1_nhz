#ifndef INTERACTABLESYSTEMS_H
#define INTERACTABLESYSTEMS_H

#include "ECS.h"
#include "EditorSystems.h"
//#include "debugmalloc.h"


typedef struct InteractionData_collision {
	int entityID;
	Vec2 collisionNormal;
	GameState* gameState;
} InteractionData_collision;

typedef struct InteractionData_overlap {
	int entityID;
} InteractionData_overlap;

typedef struct InteractionData_click {
	Vec2 mousePosition;
} InteractionData_click;

typedef struct InteractionData_hold {
	Vec2 mousePosition;
} InteractionData_hold;

typedef struct InteractionData_keydown {
	char key;
	Uint16 mod;
	int specialKey;

	Layout** layoutsPtr;
	int* numberOfLayoutsPtr;
	GameResources* gameResourcesPtr;
	char* playerName;
	Layout** currentLayoutPtr;
} InteractionData_keydown;

void Interactable_interact(Interactable* interactable, InteractionType interactionType, void* interactionData);
void Interactable_update(Layout* layouts, int numberOfLayouts, char* currentLayoutName, Interactable* interactable, GameState gameState);
void Interactable_resolveInteraction(Layout* layouts, int numberOfLayouts, char* currentLayoutName, Interactable* interactable, Interaction* interaction, GameState gameState, bool* activeInteractions);

void Interactable_loseFocusForAll(Layout* currentLayout);
void Interactable_focus(Layout* currentLayout, int entityID);
void Interactable_focusEntityAtClick(Layout* currentLayout, Vec2 mousePosition);
Interactable* Interactabe_getFocused(Layout* currentLayout);

#endif // !INTERACTABLE_H
