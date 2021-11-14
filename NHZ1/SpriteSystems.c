#include "SpriteSystems.h"


void Sprite_render(Layout* currentLayout, Sprite* sprite, SDL_Renderer* renderer) {
	if (sprite->renderProps.isVisible) {
		Position* position = ECS_getComponent(POSITION, *currentLayout, sprite->ENTITY_ID);
		if (NULL == position) {
			SDL_Log("To render a sprite, the sprite's parent entity has to have a position component. ");
			exit(1);
		}

		SDL_Rect dstrect = {
			position->value.x, position->value.y,
			round(sprite->tilemap->tileSize.x * sprite->renderProps.scale), round(sprite->tilemap->tileSize.y * sprite->renderProps.scale) };
		SDL_Rect srcrect = Tilemap_getSourceRect(sprite->tilemap, sprite->tilePosition);

		SDL_RenderCopy(renderer, sprite->tilemap->texture, &srcrect, &dstrect);
	}
}

bool Sprite_isPointInside(ComponentLists* components, Sprite* sprite, Vec2 point)
{
	Position *pos;
	pos = ECS_getPositionComponent(components, sprite->ENTITY_ID);
	if (NULL == pos) {
		SDL_Log("To render a sprite, the sprite's parent entity has to have a position component. ");
		exit(1);
	}

	Vec2 wh = { sprite->tilemap->tileSize.x * sprite->renderProps.scale , sprite->tilemap->tileSize.y * sprite->renderProps.scale };

	return (point.x > pos->value.x && point.x < pos->value.x + wh.x) && (point.y > pos->value.y && point.y < pos->value.y + wh.y);
}

void Sprite_edit_update(ComponentLists* components, Sprite* sprite, SDL_Renderer* renderer)
{
	//int x, y;
	//Uint32 buttons;
	//SDL_PumpEvents();
	//buttons = SDL_GetMouseState(&x, &y);

	//Editor* editor = ECS_getEditorComponent(components, sprite->ENTITY_ID);
	//if (NULL == editor) {
	//	SDL_Log("To update a sprite in edit mode, the sprite's parent entity has to have a Editor component. ");
	//	exit(1);
	//}
	//Position* position = ECS_getPositionComponent(components, sprite->ENTITY_ID);
	//if (NULL == position) exit(1);

	//if ((buttons & SDL_BUTTON_LMASK) != 0) {
	//	if (Sprite_isPointInside(components, sprite, (Vec2) { x, y })) {
	//		if (editor->isSelected == false) {
	//			ECS_printEntityData(components, sprite->ENTITY_ID);
	//			Editor_select(components, sprite->ENTITY_ID);
	//		}
	//	}
	//}
}
