#include "CollisionBoxSystems.h"

void CollisionBox_update(int gameState, Layout* currentLayout, CollisionBox* collisionBox) {
	void** entity = ECS_getEntity(*currentLayout, collisionBox->ENTITY_ID);
	Tile* tile = entity[TILE];
	if (NULL != tile) {
		collisionBox->size = (Vec2){ tile->tilemap->tileSize.x * tile->size.x, tile->tilemap->tileSize.y * tile->size.y};
	}
	Sprite* sprite = entity[SPRITE];
	if (NULL != sprite) {
		collisionBox->size = (Vec2){ sprite->tilemap->tileSize.x, sprite->tilemap->tileSize.y };
	}
	Text* text = entity[TEXT];
	if (NULL != text) {
		collisionBox->size = (Vec2){ text->textBoxSize.x, text->textBoxSize.y };
	}

	ECS_freeEntity(entity);
}

bool CollisionBox_isPointInside(Layout* currentLayout, CollisionBox* collisionBox, Vec2 point)
{
	Position* pos = ECS_getComponent(POSITION, *currentLayout, collisionBox->ENTITY_ID);
	if (NULL == pos) exit(1);

	return (point.x > pos->value.x && point.x < pos->value.x + collisionBox->size.x) && (point.y > pos->value.y && point.y < pos->value.y + collisionBox->size.y);
}

bool CollisionBox_checkForOverlapp(SDL_Rect A, SDL_Rect B) {
	return (A.x < B.x + B.w && A.x + A.w > B.x
		&& A.y < B.y + B.h && A.y + A.h > B.y);
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
