#include "EditorSystems.h"


void Editor_render(int gameState, ComponentLists* components, Editor* editor, SDL_Renderer* renderer) {
	if (gameState == EDIT_MODE) {
		if (editor->isSelected == true || editor->copied == true) {
			Position* position = ECS_getPositionComponent(components, editor->ENTITY_ID);
			if (NULL == position) exit(1);
			CollisionBox* collisionBox = ECS_getCollisionBoxComponent(components, editor->ENTITY_ID);
			if (NULL == collisionBox) exit(1);

			SDL_Rect rect = { .x = position->value.x - 1, .y = position->value.y - 1,
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

		Editor_renderPositionData(components, editor, renderer);
	}
}

void Editor_renderPositionData(ComponentLists* components, Editor* editor, SDL_Renderer* renderer) {
	if (editor->isSelected == true) {
		Position* position = ECS_getPositionComponent(components, editor->ENTITY_ID);
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

		// init textbox
		SDL_Rect position_rect = {
			.x = position->value.x, .y = position->value.y - positionSurface->h - 3,
			.w = positionSurface->w, .h = positionSurface->h };
		SDL_Rect ID_rect = {
		.x = position->value.x, .y = position->value.y,
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

void Editor_copy(ComponentLists* components, int entityID)
{
	for (int i = 0; i < components->total_editorComponents; i++) {
		if (components->editorComponents[i].ENTITY_ID == entityID)
			components->editorComponents[i].copied = true;
		else
			components->editorComponents[i].copied = false;
	}
}

void Editor_paste(ComponentLists* components, int newEntityID) {
	int selected = Editor_getSelected(components);
	int copied = 0;
	for (int i = 0; i < components->total_editorComponents; i++)
		if (components->editorComponents[i].copied == true) copied = components->editorComponents[i].ENTITY_ID;
	if (NULL == copied) return;

	Position* position;
	if (selected != NULL)
		position = ECS_getPositionComponent(components, selected); 
	else
		position = ECS_getPositionComponent(components, copied);
	if (NULL != position) {
		Position_init(newEntityID, &components->total_positionComponents, components->positionComponents, (Vec2) { position->value.x + 12, position->value.y + 12 });
	}

	Sprite* sprite = ECS_getSpriteComponent(components, copied);
	if (NULL != sprite) {
		Sprite_init(components->spriteComponents, newEntityID, &components->total_spriteComponents, sprite->tilemap, sprite->tilePosition);
		ECS_getSpriteComponent(components, newEntityID)->renderProps = sprite->renderProps;
	}

	Animation* animation = ECS_getAnimationComponent(components, copied);
	if (NULL != animation) {
		Animation_init(components->animationComponents, newEntityID, &components->total_animationComponents, animation->tilePosition, animation->frameCount, animation->animationSpeed);
		ECS_getAnimationComponent(components, newEntityID)->currentFrame = animation->currentFrame;
		ECS_getAnimationComponent(components, newEntityID)->lastUpdateTime = animation->lastUpdateTime;
	}

	Tile* tile = ECS_getTileComponent(components, copied);
	if (NULL != tile) {
		Tile_init(components->tileComponents, newEntityID, &components->total_tileComponents, tile->tilemap, tile->tilePosition, tile->size);
		ECS_getTileComponent(components, newEntityID)->renderProps = tile->renderProps;
	}

	Text* text = ECS_getTextComponent(components, copied);
	if (NULL != text) {
		Text_init(components->textComponents, newEntityID, &components->total_textComponents, "", text->fontSize, "", text->fontColor, text->textBoxSize);
		ECS_getTextComponent(components, newEntityID)->renderProps = text->renderProps;
		strcpy_s(ECS_getTextComponent(components, newEntityID)->value, 255, text->value, 255);
		strcpy_s(ECS_getTextComponent(components, newEntityID)->fontFamily, 255, text->fontFamily, 255);
	}

	CollisionBox* collisionBox = ECS_getCollisionBoxComponent(components, copied);
	if (NULL != collisionBox) {
		CollisionBox_init(components->collisionBoxComponents, newEntityID, &components->total_collisionBoxComponents, collisionBox->size);
	}

	Collider* collider = ECS_getColliderComponent(components, copied);
	if (NULL != collider) {
		Collider_init(components->colliderComponents, newEntityID, &components->total_colliderComponents, collider->type);
	}

	PhysicsBody* physicsBody = ECS_getPhysicsBodyComponent(components, copied);
	if (NULL != physicsBody) {
		PhysicsBody_init(components->physicsBodyComponents, newEntityID, &components->total_physicsBodyComponents, physicsBody->mass);
		ECS_getPhysicsBodyComponent(components, newEntityID)->gravitationalAcceleration = physicsBody->gravitationalAcceleration;
		ECS_getPhysicsBodyComponent(components, newEntityID)->velocity = physicsBody->velocity;
		ECS_getPhysicsBodyComponent(components, newEntityID)->acceleration = physicsBody->acceleration;
	}

	Editor_init(components->editorComponents, newEntityID, &components->total_editorComponents);
	Editor_select(components, newEntityID);
}
