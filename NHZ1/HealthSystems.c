#include "HealthSystems.h"

void Health_update(Layout* layouts, int numberOfLayouts, char* layoutName, Health* health, double delta_t, GameState* gameState) {
	Layout* currentLayout = ECS_getLayout(layouts, numberOfLayouts, layoutName);
	
	// stop invulnerablility after 2 seconds
	health->timeSinceLastDamage += delta_t;
	if (health->timeSinceLastDamage > 2) {
		health->isInvulnerable = false;
		Sprite* sprite = ECS_getComponent(SPRITE, *currentLayout, health->ENTITY_ID);
		if (sprite != NULL) sprite->renderProps.isVisible = true;
	}

	if (health->value == 0) {
		// delete enemy when it's heatlth is set to 0
		Interactable* interactable = ECS_getComponent(INTERACTABLE, *currentLayout, health->ENTITY_ID);
		if (interactable != NULL && interactable->type == ENEMY) {
			void** enemy = ECS_getEntity(*currentLayout, interactable->ENTITY_ID);

			for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++)
				if (enemy[i] != NULL) ECS_deleteComponent(i, layouts, numberOfLayouts, layoutName, interactable->ENTITY_ID);
		}

		// restart game when the player's health is 0. 
		if (health->ENTITY_ID == 1)
			*gameState = RESTART;
	}

	// flash invulnerable characters
	if (health->isInvulnerable) {
		Sprite* sprite = ECS_getComponent(SPRITE, *currentLayout, health->ENTITY_ID);
		if (sprite != NULL) sprite->renderProps.isVisible = (int)floor(health->timeSinceLastDamage * 10.0) % 3 >= 1;
	}
}
