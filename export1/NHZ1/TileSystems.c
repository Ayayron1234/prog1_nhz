#include "TileSystems.h"


void Tile_render(ComponentLists* components, Tile* tile, SDL_Renderer* renderer) {
	if (tile->renderProps.isVisible) {
		Position* position = ECS_getPositionComponent(components, tile->ENTITY_ID);
		if (NULL == position) {
			SDL_Log("To render a tile, the tile's parent entity has to have a position component. ");
			exit(1);
		}

		SDL_Rect srcrect = Tilemap_getSourceRect(tile->tilemap, tile->tilePosition);
		for (int i = 0; i < tile->size.x; i++) {
			for (int j = 0; j < tile->size.y; j++) {
				SDL_Rect dstrect = {
					position->value.x + i * round(tile->tilemap->tileSize.x * tile->renderProps.scale),
					position->value.y + j * round(tile->tilemap->tileSize.y * tile->renderProps.scale),
					round(tile->tilemap->tileSize.x * tile->renderProps.scale),
					round(tile->tilemap->tileSize.y * tile->renderProps.scale) };

				SDL_RenderCopy(renderer, tile->tilemap->texture, &srcrect, &dstrect);
			}
		}
	}
}

bool Tile_isPointInside(ComponentLists* components, Tile* tile, Vec2 point)
{
	Position* pos;
	pos = ECS_getPositionComponent(components, tile->ENTITY_ID);
	if (NULL == pos) {
		SDL_Log("To render a sprite, the sprite's parent entity has to have a position component. ");
		exit(1);
	}

	Vec2 wh = { 
		tile->tilemap->tileSize.x * tile->renderProps.scale * tile->size.x,
		tile->tilemap->tileSize.y * tile->renderProps.scale * tile->size.y
	};

	return (point.x > pos->value.x && point.x < pos->value.x + wh.x) && (point.y > pos->value.y && point.y < pos->value.y + wh.y);
}

void Tile_edit_update(ComponentLists* components, Tile* tile, SDL_Renderer* renderer)
{
	//int x, y;
	//Uint32 buttons;
	//SDL_PumpEvents();
	//buttons = SDL_GetMouseState(&x, &y);

	//Editor* editor = ECS_getEditorComponent(components, tile->ENTITY_ID);
	//if (NULL == editor) {
	//	SDL_Log("To update a sprite in edit mode, the sprite's parent entity has to have a Editor component. ");
	//	exit(1);
	//}
	//Position* position = ECS_getPositionComponent(components, tile->ENTITY_ID);
	//if (NULL == position) exit(1);

	//if ((buttons & SDL_BUTTON_LMASK) != 0) {
	//	if (Tile_isPointInside(components, tile, (Vec2) { x, y })) {
	//		if (editor->isSelected == false) {
	//			ECS_printEntityData(components, tile->ENTITY_ID);
	//			Editor_select(components, tile->ENTITY_ID);
	//		}
	//	}
	//}
}

