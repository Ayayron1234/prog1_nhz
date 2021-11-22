#include "EditorSystems.h"


void Editor_render(int gameState, Layout* currentLayout, Editor* editor, SDL_Renderer* renderer) {
	if (gameState == EDIT_MODE) {
		if (editor->isSelected == true || editor->copied == true) {
			Position* position = ECS_getComponent(POSITION, *currentLayout, editor->ENTITY_ID);
			if (NULL == position) exit(1);
			CollisionBox* collisionBox = ECS_getComponent(COLLISION_BOX, *currentLayout, editor->ENTITY_ID);
			if (NULL == collisionBox) exit(1);

			Vec2 parallax = { 0, 0 };
			EntityRenderer* entityRenderer = ECS_getComponent(ENTITY_RENDERER, *currentLayout, editor->ENTITY_ID);
			if (NULL != entityRenderer) parallax = currentLayout->layers[entityRenderer->layerIndex].parallax;

			SDL_Rect rect = { .x = (position->value.x - 1) - currentLayout->camera.x * parallax.x, .y = (position->value.y - 1) - currentLayout->camera.y * parallax.y,
				.w = collisionBox->size.x + 2, .h = collisionBox->size.y + 2 };

			if (editor->isSelected)
				SDL_SetRenderDrawColor(renderer, 136, 252, 3, 80);
			else
				SDL_SetRenderDrawColor(renderer, 250, 105, 255, 120);
			SDL_RenderFillRect(renderer, &rect);
			if (editor->isSelected)
				SDL_SetRenderDrawColor(renderer, 136, 252, 3, 255);
			else
				SDL_SetRenderDrawColor(renderer, 250, 105, 255, 255);
			SDL_RenderDrawRect(renderer, &rect);
		}

		Editor_renderPositionData(currentLayout, editor, renderer);
	}
}

void Editor_renderPositionData(Layout* currentLayout, Editor* editor, SDL_Renderer* renderer) {
	if (editor->isSelected == true) {
		Position* position = ECS_getComponent(POSITION, *currentLayout, editor->ENTITY_ID);
		if (NULL == position) exit(1);

		// create font properties
		TTF_Font* font = TTF_OpenFont("Sans.ttf", 14);
		SDL_Color color = { 200, 200, 200, };
	
		// initiate surfaces
		// position data:
		char positionText[255];
		sprintf_s(&positionText, 255, "(x=%g; y=%g)", position->value.x, position->value.y);
		SDL_Surface* positionSurface = TTF_RenderText_Solid(font, positionText, color);
		// entity id
		char IDText[255];
		sprintf_s(&IDText, 255, "#%d", position->ENTITY_ID);
		SDL_Surface* IDSurface = TTF_RenderText_Solid(font, IDText, color);
	
		// convert surface to texture
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, positionSurface);
		SDL_Texture* IDTexture = SDL_CreateTextureFromSurface(renderer, IDSurface);

		// get parallax
		Vec2 parallax = { 0, 0 };
		EntityRenderer* entityRenderer = ECS_getComponent(ENTITY_RENDERER, *currentLayout, editor->ENTITY_ID);
		if (NULL != entityRenderer) parallax = currentLayout->layers[entityRenderer->layerIndex].parallax;

		// init textbox
		SDL_Rect position_rect = {
			.x = position->value.x - currentLayout->camera.x * parallax.x, .y = position->value.y - positionSurface->h - 3 - currentLayout->camera.y * parallax.y,
			.w = positionSurface->w, .h = positionSurface->h };
		SDL_Rect ID_rect = {
		.x = position->value.x - currentLayout->camera.x * parallax.x, .y = position->value.y - currentLayout->camera.y * parallax.y,
		.w = IDSurface->w, .h = IDSurface->h };


		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
		SDL_RenderFillRect(renderer, &position_rect);
		SDL_RenderFillRect(renderer, &ID_rect);

		// render texture
		SDL_RenderCopy(renderer, textTexture, NULL, &position_rect);
		SDL_RenderCopy(renderer, IDTexture, NULL, &ID_rect);

		// free texture and surface
		SDL_FreeSurface(positionSurface);
		SDL_FreeSurface(IDSurface);
		SDL_DestroyTexture(textTexture);
		SDL_DestroyTexture(IDTexture);
		TTF_CloseFont(font);
	}
}

void Editor_update(GameState gameState, Layout* currentLayout, Editor* editor) {
	//if (gameState == EDIT_MODE) {
	//	// select clicked entity
	//	void** entity = ECS_getEntity(*currentLayout, editor->ENTITY_ID);

	//	// get position and collision box data from entity
	//	Position* position = entity[POSITION];
	//	CollisionBox* collisionBox = entity[COLLISION_BOX];

	//	if (NULL != position && NULL != collisionBox) {
	//		// get mouse position
	//		Uint32 buttons;
	//		Vec2Int mousePos;
	//		SDL_PumpEvents();
	//		buttons = SDL_GetMouseState(&mousePos.x, &mousePos.y);

	//		// check if left clicking
	//		if ((buttons & SDL_BUTTON_LMASK) != 0) {
	//			// check if cursor is over entity
	//			if (CollisionBox_isPointInside(currentLayout, collisionBox, (Vec2) { mousePos.x, mousePos.y })) {
	//				// check if entity isn't allready selected
	//				if (editor->isSelected == false) {
	//					ECS_printEntityData(currentLayout, collisionBox->ENTITY_ID);
	//					Editor_select(currentLayout, collisionBox->ENTITY_ID);
	//				}
	//			}
	//		}
	//	}

	//	ECS_freeEntity(entity);
	//}
}

void Editor_selectEntityAtClick(Layout* currentLayout, Vec2 mousePosition) {
	bool nothingClicked = true;
	for (int i = 0; i < ECS_getNumberOfComponents(COLLISION_BOX, *currentLayout); i++) {
		CollisionBox* collisionBox = ECS_getNthComponent(COLLISION_BOX, currentLayout, i);

		// transform mouse position based on the camera's position and the entity's parent layer's parallax
		Vec2 parallax = { 0, 0 };
		EntityRenderer* entityRenderer = ECS_getComponent(ENTITY_RENDERER, *currentLayout, collisionBox->ENTITY_ID);
		if (NULL != entityRenderer) parallax = currentLayout->layers[entityRenderer->layerIndex].parallax;
		Vec2 transformedPosition = Vec2_add(mousePosition, (Vec2) { currentLayout->camera.x * parallax.x, currentLayout->camera.y * parallax.y });

		if (CollisionBox_isPointInside(currentLayout, collisionBox, transformedPosition)) {
			nothingClicked = false;

			Editor* editor = ECS_getComponent(EDITOR, *currentLayout, collisionBox->ENTITY_ID);

			if (!editor->isSelected) {
				Editor_select(currentLayout, editor->ENTITY_ID);
				ECS_printEntityData(currentLayout, editor->ENTITY_ID);
			} 			
		}
	}

	if (nothingClicked) Editor_deselectAll(currentLayout);
}

int Editor_getSelected(Layout* currentLayout) {
	for (int i = 0; i < ECS_getNumberOfComponents(EDITOR, *currentLayout); i++) {
		if (((Editor*)ECS_getNthComponent(EDITOR, currentLayout, i))->isSelected == true)
			return ((Editor*)ECS_getNthComponent(EDITOR, currentLayout, i))->ENTITY_ID;
	}
	return 0;
}

void Editor_select(Layout* currentLayout, int entityID) {
	for (int i = 0; i < ECS_getNumberOfComponents(EDITOR, *currentLayout); i++) {
		if (((Editor*)ECS_getNthComponent(EDITOR, currentLayout, i))->ENTITY_ID == entityID)
			((Editor*)ECS_getNthComponent(EDITOR, currentLayout, i))->isSelected = true;
		else
			((Editor*)ECS_getNthComponent(EDITOR, currentLayout, i))->isSelected = false;
	}
}

void Editor_deselectAll(Layout* currentLayout) {
	for (int i = 0; i < ECS_getNumberOfComponents(EDITOR, *currentLayout); i++)
		((Editor*)ECS_getNthComponent(EDITOR, currentLayout, i))->isSelected = false;
}

void Editor_copy(Layout* currentLayout, int entityID)
{
	for (int i = 0; i < ECS_getNumberOfComponents(EDITOR, *currentLayout); i++) {
		Editor* editor = ECS_getNthComponent(EDITOR, currentLayout, i);
		if (editor->ENTITY_ID == entityID)
			editor->copied = true;
		else
			editor->copied = false;
	}
}

void Editor_paste(Layout* layouts, int numberOfLayouts, char* layoutName) {
	Layout* currentLayout = ECS_getLayout(layouts, numberOfLayouts, layoutName);
	
	// get first free entity on the target layout
	int newEntityID = ECS_getFreeID(currentLayout);

	// get selected entity
	int selectedID = Editor_getSelected(currentLayout);
	if (selectedID == 0) return;
	void** selected = ECS_getEntity(*currentLayout, selectedID);
	if (selected != NULL)
		((Editor*)selected[EDITOR])->isSelected = false;

	// get copied entity
	int copiedID = 0;
	int layoutIndex;
	for (layoutIndex = 0; layoutIndex < numberOfLayouts; layoutIndex++) {
		for (int i = 0; i < ECS_getNumberOfComponents(EDITOR, layouts[layoutIndex]); i++)
			if (((Editor*)ECS_getNthComponent(EDITOR, &layouts[layoutIndex], i))->copied == true) {
				copiedID = ((Editor*)ECS_getNthComponent(EDITOR, &layouts[layoutIndex], i))->ENTITY_ID;
				break;
			}
		if (copiedID != 0) break;
	}
	if (NULL == copiedID) return;
	void** copied = ECS_getEntity(layouts[layoutIndex], copiedID);
	if (copied == NULL) return;

	// create new component
	for (int componentTypeIndex = 0; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) {
		if (copied[componentTypeIndex] != NULL) {
			void* newComponent = ECS_createComponent(componentTypeIndex, layouts, numberOfLayouts, layoutName, newEntityID);
			memcpy(newComponent, copied[componentTypeIndex], ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL));

			*ECS_getEntityIDPtr(componentTypeIndex, newComponent) = newEntityID;

			if (componentTypeIndex == POSITION && selected != NULL)
				((Position*)newComponent)->value = Vec2_add(((Position*)selected[POSITION])->value, (Vec2){ 12, 12 });

			if (componentTypeIndex == EDITOR) {
				((Editor*)newComponent)->copied = false;
				((Editor*)newComponent)->isSelected = true;
			}
		}
	}

	ECS_freeEntity(selected);
	ECS_freeEntity(copied);
}
