#include "AnimationSystems.h"


void Animation_update(Layout currentLayout , Animation* animation)
{
	if (animation->lastUpdateTime == 0) {
		animation->lastUpdateTime = SDL_GetTicks();
		return;
	}
	int a = animation->lastUpdateTime - SDL_GetTicks();
	if (SDL_GetTicks() - animation->lastUpdateTime > 1000 / animation->animationSpeed) {
		animation->lastUpdateTime = SDL_GetTicks();
		
		Sprite* sprite = ECS_getComponent(SPRITE, currentLayout, animation->ENTITY_ID);
		if (NULL == sprite) exit(1);

		if (animation->currentFrame < animation->frameCount - 1)
			animation->currentFrame++;
		else
			animation->currentFrame = 0;

		sprite->tilePosition.x = animation->tilePosition.x + animation->currentFrame;
		sprite->tilePosition.y = animation->tilePosition.y;
	}	
}
