#include "TileSystems.h"


void Tile_render(Layout* currentLayout, Tile* tile, SDL_Renderer* renderer) {
	if (tile->renderProps.isVisible) {
		Position* position = ECS_getComponent(POSITION, *currentLayout, tile->ENTITY_ID);
		if (NULL == position) {
			SDL_Log("To render a tile, the tile's parent entity has to have a position component. ");
			exit(1);
		}

		Vec2 parallax = { 0, 0 };
		EntityRenderer* entityRenderer = ECS_getComponent(ENTITY_RENDERER, *currentLayout, tile->ENTITY_ID);
		if (NULL != entityRenderer) parallax = currentLayout->layers[entityRenderer->layerIndex].parallax;

		SDL_Rect srcrect = Tilemap_getSourceRect(tile->tilemap, tile->tilePosition);
		for (int i = 0; i < tile->size.x; i++) {
			for (int j = 0; j < tile->size.y; j++) {
				SDL_Rect dstrect = {
					(position->value.x + i * round(tile->tilemap->tileSize.x * tile->renderProps.scale)) - (currentLayout->camera.x * parallax.x),
					(position->value.y + j * round(tile->tilemap->tileSize.y * tile->renderProps.scale)) - (currentLayout->camera.y * parallax.y),
					round(tile->tilemap->tileSize.x * tile->renderProps.scale),
					round(tile->tilemap->tileSize.y * tile->renderProps.scale) };

				SDL_RenderCopy(renderer, tile->tilemap->texture, &srcrect, &dstrect);
			}
		}
	}
}

//bool Tile_isPointInside(ComponentLists* components, Tile* tile, Vec2 point)
//{
//	Position* pos;
//	pos = ECS_getPositionComponent(components, tile->ENTITY_ID);
//	if (NULL == pos) {
//		SDL_Log("To render a sprite, the sprite's parent entity has to have a position component. ");
//		exit(1);
//	}
//
//	Vec2 wh = { 
//		tile->tilemap->tileSize.x * tile->renderProps.scale * tile->size.x,
//		tile->tilemap->tileSize.y * tile->renderProps.scale * tile->size.y
//	};
//
//	return (point.x > pos->value.x && point.x < pos->value.x + wh.x) && (point.y > pos->value.y && point.y < pos->value.y + wh.y);
//}

//void Tile_edit_update(ComponentLists* components, Tile* tile, SDL_Renderer* renderer) {
//
//}

