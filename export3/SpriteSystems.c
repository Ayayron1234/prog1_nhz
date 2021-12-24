#include "SpriteSystems.h"


void Sprite_render(Layout* currentLayout, Sprite* sprite, SDL_Renderer* renderer) {
	if (sprite->renderProps.isVisible) {
		Position* position = ECS_getComponent(POSITION, *currentLayout, sprite->ENTITY_ID);
		if (NULL == position) {
			SDL_Log("To render a sprite, the sprite's parent entity has to have a position component. ");
			exit(1);
		}

		SDL_Rect dstrect = {
			(position->value.x) - currentLayout->camera.x, (position->value.y) - currentLayout->camera.y,
			round(sprite->tilemap->tileSize.x * sprite->renderProps.scale), round(sprite->tilemap->tileSize.y * sprite->renderProps.scale) };
		SDL_Rect srcrect = Tilemap_getSourceRect(sprite->tilemap, sprite->tilePosition);

		SDL_RenderCopy(renderer, sprite->tilemap->texture, &srcrect, &dstrect);
	}
}

//bool Sprite_isPointInside(ComponentLists* components, Sprite* sprite, Vec2 point)
//{
//	Position *pos;
//	pos = ECS_getPositionComponent(components, sprite->ENTITY_ID);
//	if (NULL == pos) {
//		SDL_Log("To render a sprite, the sprite's parent entity has to have a position component. ");
//		exit(1);
//	}
//
//	Vec2 wh = { sprite->tilemap->tileSize.x * sprite->renderProps.scale , sprite->tilemap->tileSize.y * sprite->renderProps.scale };
//
//	return (point.x > pos->value.x && point.x < pos->value.x + wh.x) && (point.y > pos->value.y && point.y < pos->value.y + wh.y);
//}

//void Sprite_edit_update(ComponentLists* components, Sprite* sprite, SDL_Renderer* renderer) {
//
//}
