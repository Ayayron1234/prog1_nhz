#include "EditorSystems.h"


void Editor_render(ComponentLists* components, Editor* editor, SDL_Renderer* renderer) {
	if (ECS_getSpriteComponent(components, editor->ENTITY_ID) != NULL)
		Editor_renderForSprite(components, editor, renderer);
	if (ECS_getTileComponent(components, editor->ENTITY_ID) != NULL)
		Editor_renderForTile(components, editor, renderer);
}

void Editor_renderForSprite(ComponentLists* components, Editor* editor, SDL_Renderer* renderer) {
	if (editor->isSelected == true) {
		Position* position = ECS_getPositionComponent(components, editor->ENTITY_ID);
		if (NULL == position) exit(1);
		Sprite* sprite = ECS_getSpriteComponent(components, editor->ENTITY_ID);
		if (NULL == sprite) exit(1);

		SDL_Rect rect = { .x = position->value.x - 1, .y = position->value.y - 1,
			.w = sprite->tilemap->tileSize.x + 2, .h = sprite->tilemap->tileSize.y + 2 };

		SDL_SetRenderDrawColor(renderer, 232, 252, 3, 80);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 232, 252, 3, 255);
		SDL_RenderDrawRect(renderer, &rect);
	}
}
void Editor_renderForTile(ComponentLists* components, Editor* editor, SDL_Renderer* renderer) {
	if (editor->isSelected == true) {
		Position* position = ECS_getPositionComponent(components, editor->ENTITY_ID);
		if (NULL == position) exit(1);
		Tile* tile = ECS_getTileComponent(components, editor->ENTITY_ID);
		if (NULL == tile) exit(1);

		SDL_Rect rect = { .x = position->value.x - 1, .y = position->value.y - 1,
			.w = tile->tilemap->tileSize.x * tile->size.x + 1, 
			.h = tile->tilemap->tileSize.y * tile->size.y + 1
		};

		SDL_SetRenderDrawColor(renderer, 232, 252, 3, 80);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 232, 252, 3, 255);
		SDL_RenderDrawRect(renderer, &rect);
	}
}

int Editor_getSelected(ComponentLists* components) {
	for (int i = 0; i < components->total_editorComponents; i++) {
		if (components->editorComponents[i].isSelected == true)
			return components->editorComponents[i].ENTITY_ID;
	}
	return 0;
}

void Editor_select(ComponentLists* components, int entityID) {
	for (int i = 0; i < components->total_editorComponents; i++) {
		if (components->editorComponents[i].ENTITY_ID == entityID)
			components->editorComponents[i].isSelected = true;
		else
			components->editorComponents[i].isSelected = false;
	}
}

void Editor_deselectAll(ComponentLists* components) {
	for (int i = 0; i < components->total_editorComponents; i++) {
		components->editorComponents[i].isSelected = false;
	}
}
