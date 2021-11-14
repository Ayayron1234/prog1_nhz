#include "CollisionBoxSystems.h"

void CollisionBox_update(int gameState, ComponentLists* components, CollisionBox* collisionBox) {
	Tile* tile = ECS_getTileComponent(components, collisionBox->ENTITY_ID);
	if (NULL != tile) {
		collisionBox->size = (Vec2){ tile->tilemap->tileSize.x * tile->size.x, tile->tilemap->tileSize.y * tile->size.y};
	}
	Sprite* sprite = ECS_getSpriteComponent(components, collisionBox->ENTITY_ID);
	if (NULL != sprite) {
		collisionBox->size = (Vec2){ sprite->tilemap->tileSize.x, sprite->tilemap->tileSize.y };
	}
	Text* text = ECS_getTextComponent(components, collisionBox->ENTITY_ID);
	if (NULL != text) {
		collisionBox->size = (Vec2){ text->textBoxSize.x, text->textBoxSize.y };
	}

	if (gameState == EDIT_MODE) {
		int x, y;
		Uint32 buttons;
		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&x, &y);

		Editor* editor = ECS_getEditorComponent(components, collisionBox->ENTITY_ID);
		if (NULL == editor) {
			SDL_Log("To update a sprite in edit mode, the sprite's parent entity has to have a Editor component. ");
			exit(1);
		}
		Position* position = ECS_getPositionComponent(components, collisionBox->ENTITY_ID);
		if (NULL == position) exit(1);

		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			if (CollisionBox_isPointInside(components, collisionBox, (Vec2) { x, y })) {
				if (editor->isSelected == false) {
					ECS_printEntityData(components, collisionBox->ENTITY_ID);
					Editor_select(components, collisionBox->ENTITY_ID);
				}
			}
		}
	}
}

bool CollisionBox_isPointInside(Layout* currentLayout, CollisionBox* collisionBox, Vec2 point)
{
	Position* pos = ECS_getComponent(POSITION, *currentLayout, collisionBox->ENTITY_ID);
	if (NULL == pos) {
		exit(1);
	}

	return (point.x > pos->value.x && point.x < pos->value.x + collisionBox->size.x) && (point.y > pos->value.y && point.y < pos->value.y + collisionBox->size.y);
}

bool CollisionBox_checkForOverlapp(ComponentLists* components, CollisionBox* collisionBoxA, CollisionBox* collisionBoxB) {
	Position *posA, *posB;
	posA = ECS_getPositionComponent(components, collisionBoxA->ENTITY_ID);
	if (NULL == posA) exit(1);
	posB = ECS_getPositionComponent(components, collisionBoxB->ENTITY_ID);
	if (NULL == posB) exit(1);

	return (posA->value.x < posB->value.x + collisionBoxB->size.x && posA->value.x + collisionBoxA->size.x > posB->value.x
		&& posA->value.y < posB->value.y + collisionBoxB->size.y && posA->value.y + collisionBoxA->size.y > posB->value.y);
}

// ezt a tutorialt követtem: https://www.youtube.com/watch?v=8JJ-4JgR7Dg&t=790s
bool CollisionBox_checkIfRayOverlapps(Layout* currentLayout, CollisionBox* collisionBox, Ray2 ray, Vec2* contactPoint, Vec2* contactNormal, double* tHitNear) {
	Position* pos = ECS_getComponent(POSITION, *currentLayout, collisionBox->ENTITY_ID);
	if (NULL == pos) return false;

	*contactNormal = (Vec2){ 0, 0 };
	*contactPoint = (Vec2){ 0, 0 };

	Vec2 invDir = { 1.0 / ray.destination.x, 1.0 / ray.destination.y };

	Vec2 tNear = { (pos->value.x - ray.origin.x) * invDir.x, (pos->value.y - ray.origin.y) * invDir.y };
	Vec2 tFar = { (pos->value.x + collisionBox->size.x - ray.origin.x) * invDir.x, (pos->value.y + collisionBox->size.y - ray.origin.y) * invDir.y };
	
	if (isnan(tFar.y) || isnan(tFar.x)) return false;
	if (isnan(tNear.y) || isnan(tNear.x)) return false;

	if (tNear.x > tFar.x) {
		double temp = tNear.x;
		tNear.x = tFar.x;
		tFar.x = temp;
	}
	if (tNear.y > tFar.y) {
		double temp = tNear.y;
		tNear.y = tFar.y;
		tFar.y = temp;
	}

	if (tNear.x > tFar.y || tNear.y > tFar.x) return false;

	*tHitNear = Math_maxDouble(2, tNear.x, tNear.y);
	double tHitFar = Math_minDouble(2, tFar.x, tFar.y);

	if (tHitFar <= 0) return false;

	*contactPoint = (Vec2){
		ray.origin.x + *tHitNear * ray.destination.x,
		ray.origin.y + *tHitNear * ray.destination.y
	};

	if (tNear.x > tNear.y) {
		if (invDir.x < 0)
			*contactNormal = (Vec2){ 1, 0 };
		else
			*contactNormal = (Vec2){ -1, 0 };
	}
	else if (tNear.x < tNear.y)
		if (invDir.y < 0)
			*contactNormal = (Vec2){ 0, 1 };
		else 
			*contactNormal = (Vec2){ 0, -1 };

	return true;
}
