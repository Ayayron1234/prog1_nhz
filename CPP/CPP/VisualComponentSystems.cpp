#include "VisualComponent.h"
#include "ECS.h"

using namespace ECS;

void VisualComponent::Draw(SDL_Renderer *renderer, Vec2 transform) {
	if (this->isVisible) {
		//TODO: get position
		Vec2 position(100, 100);

		Vec2 positionOnWindow = transform + this->offset + position;
		this->dstRect.x = positionOnWindow.x;
		this->dstRect.y = positionOnWindow.y;

		SDL_RenderCopy(renderer, this->texture, &this->srcRect, &this->dstRect);
	}
}

void VisualComponent::UseTexture(SDL_Texture* texture) {
	int w, h;
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);

	this->srcRect.w = w;
	this->srcRect.h = h;

	this->dstRect.w = w;
	this->dstRect.h = h;
}
