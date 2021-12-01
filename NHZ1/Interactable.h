#ifndef INTERACTABLE_H
#define INTERACTABLE_H

#include <stdbool.h>
#include "Math.h"


#define NUMBER_OF_INTERACTION_TYPES 5
typedef enum InteractionType {
	CLICK = 0,
	COLLISION = 1,
	OVERLAP = 2,
	HOLD = 3,
	KEYDOWN = 4
} InteractionType;

typedef struct Interaction {
	InteractionType type;
	void* data;

	InteractionType nextInteractionType;
	struct Interaction* nextInteraction;
} Interaction;

typedef enum InteractableType {
	BUTTON, TEXTBOX, LUCKY_BLOCK, ENEMY, HEALTH_PICKUP, COIN, FINISH
} InteractableType;

typedef struct Interactable {
	int ENTITY_ID;

	InteractableType type;
	// focus is transfered to this interactable when tab is pressed
	int nextInFocus;
	
	bool hasFocus;
	bool activeInteractions[NUMBER_OF_INTERACTION_TYPES];
	
	Interaction* interactionHead;
	Interaction* latestInteraction;
} Interactable;

#endif // !INTERACTABLE_H
