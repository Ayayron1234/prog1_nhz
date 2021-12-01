#include "InteractableSystems.h"

void Interactable_interact(Interactable* interactable, InteractionType interactionType, void* interactionData) {
	Interaction* interaction = (Interaction*)malloc(sizeof(Interaction));
	if (NULL == interaction) return;

	interaction->data = interactionData;
	interaction->type = interactionType;
	interaction->nextInteraction = NULL;

	if (interactable->interactionHead == NULL) { 
		interactable->interactionHead = interaction;
		interactable->latestInteraction = interaction;
	} else {
		interactable->latestInteraction->nextInteraction = interaction;
		interactable->latestInteraction->nextInteractionType = interactionType;
		interactable->latestInteraction = interaction;
	}

	interactable->activeInteractions[interactionType] = true;
}

void Interactable_resolveInteraction(Layout* layouts, int numberOfLayouts, char* currentLayoutName, Interactable* interactable, Interaction* interaction, GameState gameState, bool* activeInteractions) {
	Layout* currentLayout = ECS_getLayout(layouts, numberOfLayouts, currentLayoutName);

	switch (interaction->type)
	{
	case COLLISION: {
		activeInteractions[COLLISION] = true;
		InteractionData_collision* data = interaction->data;

		switch (interactable->type)
		{
		case HEALTH_PICKUP: {
			// TODO: this is temp
			if (data->entityID != 1) break;

			void** healthPickup = ECS_getEntity(*currentLayout, interactable->ENTITY_ID);
			if (healthPickup == NULL) break;

			for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++)
				if (healthPickup[i] != NULL) ECS_deleteComponent(i, layouts, numberOfLayouts, currentLayoutName, interactable->ENTITY_ID);

			printf("Entity#%d collided whith HEALTH_PICKUP#%d\n", data->entityID, interactable->ENTITY_ID);
			break;
		}
		case LUCKY_BLOCK: {
			if (data->collisionNormal.y > 0 && data->collisionNormal.x == 0) {
				void** healthPickupTemplate = ECS_getEntity(*currentLayout, 4);
				if (NULL == healthPickupTemplate) break;

				int healthPickupID = Editor_paste(layouts, numberOfLayouts, currentLayoutName, healthPickupTemplate);
				((Position*)ECS_getComponent(POSITION, *currentLayout, healthPickupID))->value =
					Vec2_add(((Position*)ECS_getComponent(POSITION, *currentLayout, interactable->ENTITY_ID))->value, (Vec2) { 0, -60 });

				Animation* animation = ECS_getComponent(ANIMATION, *currentLayout, interactable->ENTITY_ID);
				animation->frameCount = 1;
				animation->tilePosition = (Vec2Int){ 5, 1 };

				ECS_deleteComponent(INTERACTABLE, layouts, numberOfLayouts, "map1", interactable->ENTITY_ID);

				free(healthPickupTemplate);
				printf("Entity#%d collided whith LUCKY_BLOCK#%d\n", data->entityID, interactable->ENTITY_ID);
			}
			break;
		}
		case FINISH: {
			*data->gameState = VICTORY;
			break;
		}
		default: break;
		}
		break;
	}
	case OVERLAP: {
		activeInteractions[OVERLAP] = true;
		InteractionData_overlap* data = interaction->data;

		switch (interactable->type)
		{
		default: break;
		}
		break;
	}
	case CLICK: {
		activeInteractions[CLICK] = true;
		InteractionData_click* data = interaction->data;

		switch (interactable->type)
		{
		case TEXTBOX: {
			Interactable_focus(currentLayout, interactable->ENTITY_ID);
			break;
		}
		case BUTTON: {
			Interactable_focus(currentLayout, interactable->ENTITY_ID);
			break;
			//char* buttonText = ((Text*)ECS_getComponent(TEXT, *currentLayout, interactable->ENTITY_ID))->value;
			//int x = strcmp(buttonText, "PLAY");
			//if (strcmp(((Text*)ECS_getComponent(TEXT, *currentLayout, interactable->ENTITY_ID))->value, "PLAY") == 0) {
			//	if (data->layoutsPtr != NULL && data->numberOfLayoutsPtr != NULL) {
			//		char* newPlayerName = ((Text*)ECS_getComponent(TEXT, *currentLayout, 2))->value;
			//		if (strcmp(newPlayerName, "original") == 0) break;

			//		char path[255];
			//		sprintf(path, "./saves/%s.data", newPlayerName);
			//		FILE* file;
			//		if ((file = fopen(path, "r")))
			//		{
			//			fclose(file);
			//			// file exists
			//			ECS_switchToPlayer(data->playerName, newPlayerName, data->layoutsPtr, data->numberOfLayoutsPtr, data->currentLayoutPtr, data->gameResourcesPtr);
			//		}
			//		else {
			//			// file does not exists
			//			ECS_save(*data->layoutsPtr, *data->numberOfLayoutsPtr, newPlayerName);
			//			ECS_switchToPlayer(data->playerName, newPlayerName, data->layoutsPtr, data->numberOfLayoutsPtr, data->currentLayoutPtr, data->gameResourcesPtr);
			//		}
			//	}
			//}
		}
		default:
			break;
		}
	}
	case KEYDOWN: {
		activeInteractions[KEYDOWN] = true;
		InteractionData_keydown* data = interaction->data;

		switch (interactable->type)
		{
		case TEXTBOX: {
			//if (event.key.keysym.mod & 1) printf("shift");
			//if (event.key.keysym.mod & 2) printf("shift");
			//if (event.key.keysym.mod & 64) printf("ctrl");
			//if (event.key.keysym.mod & 128) printf("ctrl");
			//if (event.key.keysym.mod & 256) printf("alt");
			//if (event.key.keysym.mod & 512) printf("alt");

			if (data->key == '\t' && interactable->nextInFocus != 0)
				Interactable_focus(currentLayout, interactable->nextInFocus);

			Text* text = ECS_getComponent(TEXT, *currentLayout, interactable->ENTITY_ID);
			if (NULL != text) {
				int length = strlen(text->value);
				// characters
				if (data->key >= 32 && data->key <= 122 && strlen(text->value) < 254) {
					text->value[length] = data->key;

					// make character capital if shift is pressed (easier this way than with toupper)
					if (data->mod & 1 || data->mod & 2) text->value[length] += 'A' - 'a';
					text->value[length + 1] = '\0';
				}
				// backspace
				if (data->key == 8 && strlen(text->value) > 0) {
					text->value[length - 1] = '\0';
				}
			}
			break;
		}
		case BUTTON: {
			// tab was pressed
			if (data->key == '\t' && interactable->nextInFocus != 0)
				Interactable_focus(currentLayout, interactable->nextInFocus);

			// enter was pressed
			if (data->specialKey == 13) {
				char* buttonText = ((Text*)ECS_getComponent(TEXT, *currentLayout, interactable->ENTITY_ID))->value;
				int x = strcmp(buttonText, "PLAY");
				if (strcmp(((Text*)ECS_getComponent(TEXT, *currentLayout, interactable->ENTITY_ID))->value, "PLAY") == 0)
					if (data->layoutsPtr != NULL && data->numberOfLayoutsPtr != NULL) {
						char* newPlayerName = ((Text*)ECS_getComponent(TEXT, *currentLayout, 2))->value;
						if (strcmp(newPlayerName, "original") == 0) break;

						char path[255];
						sprintf(path, "./saves/%s.data", newPlayerName);
						FILE* file;
						if ((file = fopen(path, "r")))
						{
							fclose(file);
							
							// file exists
							ECS_switchToPlayer(data->playerName, newPlayerName, data->layoutsPtr, data->numberOfLayoutsPtr, data->currentLayoutPtr, data->gameResourcesPtr);
						}
						else {
							// file does not exists
							ECS_save(*data->layoutsPtr, *data->numberOfLayoutsPtr, newPlayerName);
							ECS_switchToPlayer(data->playerName, newPlayerName, data->layoutsPtr, data->numberOfLayoutsPtr, data->currentLayoutPtr, data->gameResourcesPtr);
						}
					}
				printf("ENTER");
			}
		}
		default: {
			break;
		}
		}
	}
	default: break;
	}

	if (NULL != interaction->nextInteraction)
		Interactable_resolveInteraction(layouts, numberOfLayouts, currentLayoutName, interactable, interaction->nextInteraction, gameState, activeInteractions);

	free(interaction->data);
	free(interaction);
}

void Interactable_loseFocusForAll(Layout* currentLayout) {
	for (int i = 0; i < ECS_getNumberOfComponents(INTERACTABLE, *currentLayout); i++) {
		Interactable* interactable = ECS_getNthComponent(INTERACTABLE, currentLayout, i);
		interactable->hasFocus = false;
	}
}

void Interactable_focus(Layout* currentLayout, int entityID) {
	for (int i = 0; i < ECS_getNumberOfComponents(INTERACTABLE, *currentLayout); i++) {
		Interactable* interactable = ECS_getNthComponent(INTERACTABLE, currentLayout, i);
		if (interactable->ENTITY_ID == entityID)
			interactable->hasFocus = true;
		else
			interactable->hasFocus = false;
	}
}


void Interactable_focusEntityAtClick(Layout* currentLayout, Vec2 mousePosition) {
	bool nothingClicked = true;
	for (int i = 0; i < ECS_getNumberOfComponents(COLLISION_BOX, *currentLayout); i++) {
		CollisionBox* collisionBox = ECS_getNthComponent(COLLISION_BOX, currentLayout, i);

		// transform mouse position based on the camera's position and the entity's parent layer's parallax
		Vec2 parallax = { 0, 0 };
		EntityRenderer* entityRenderer = ECS_getComponent(ENTITY_RENDERER, *currentLayout, collisionBox->ENTITY_ID);
		if (NULL != entityRenderer) parallax = currentLayout->layers[entityRenderer->layerIndex].parallax;
		Vec2 transformedPosition = Vec2_add(mousePosition, (Vec2) { currentLayout->camera.x* parallax.x, currentLayout->camera.y* parallax.y });

		if (CollisionBox_isPointInside(currentLayout, collisionBox, transformedPosition)) {
			nothingClicked = false;

			Interactable* interactable = ECS_getComponent(INTERACTABLE, *currentLayout, collisionBox->ENTITY_ID);

			if (NULL != interactable && !interactable->hasFocus) {
				Interactable_focus(currentLayout, interactable->ENTITY_ID);
				ECS_printEntityData(currentLayout, interactable->ENTITY_ID);
			}
		}
	}

	if (nothingClicked) Interactable_loseFocusForAll(currentLayout);
}

Interactable* Interactabe_getFocused(Layout* currentLayout) {
	for (int i = 0; i < ECS_getNumberOfComponents(INTERACTABLE, *currentLayout); i++) {
		Interactable* interactable = ECS_getNthComponent(INTERACTABLE, currentLayout, i);
		if (interactable->hasFocus)
			return interactable;
	}
	return NULL;
}

void Interactable_update(Layout* layouts, int numberOfLayouts, char* currentLayoutName, Interactable* interactable, GameState gameState) {
	bool newActiveInteractions[NUMBER_OF_INTERACTION_TYPES] = { false };

	if (NULL != interactable->interactionHead) {
		Interactable_resolveInteraction(layouts, numberOfLayouts, currentLayoutName, interactable, interactable->interactionHead, gameState, newActiveInteractions);

		interactable->interactionHead = NULL;
		interactable->latestInteraction = NULL;
	}	

	// update active interactions
	for (int i = 0; i < NUMBER_OF_INTERACTION_TYPES; i++)
		interactable->activeInteractions[i] = newActiveInteractions[i];

	//// temp
	//Interactable* test[20];
	//for (int i = 0; i < ECS_getNumberOfComponents(INTERACTABLE, *ECS_getLayout(layouts, numberOfLayouts, currentLayoutName)); i++)
	//	test[i] = ECS_getNthComponent(INTERACTABLE, ECS_getLayout(layouts, numberOfLayouts, currentLayoutName), i);
	//printf("");
}
