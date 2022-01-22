#pragma once
#include "ComponentBaseClass.h"
#include "Vec2.h"

#include <SDL.h>
#undef main

namespace ECS {
	//class Tilemap {
	//public:
	//	int width;
	//	int height;

	//	int tileWidth;
	//	int tileHeight;
	//	SDL_Texture* texture;

	//	Tilemap(SDL_Texture* texture, int tileWidth, int tileHeight) {
	//		this->texture = texture;
	//		this->tileWidth = tileWidth;
	//		this->tileHeight = tileHeight;

	//		SDL_QueryTexture(texture, nullptr, nullptr, &this->width, &this->height);
	//	}
	//};

	class VisualComponent : public Component {
	private: 
		SDL_Texture *texture;
		SDL_Rect dstRect;
		SDL_Rect srcRect;

	public:
		bool isVisible;
		Vec2 offset;

		VisualComponent(int entity_id = 0) {
			this->ENTITY_ID = entity_id;

			this->texture = nullptr;
			this->isVisible = true;
			this->dstRect = { 0 };
			this->srcRect = { 0 };
		}

		ComponentVersionMap* getVersionMap(VisualComponent* component = nullptr) {
			if (component == nullptr) component = this;
			ComponentVersionMap* versionMap = Component::getVersionMap(this);

			versionMap
				->addMember(ComponentVersionMapMember("texture", sizeof(component->texture), &component->texture, "ptr"))
				->addMember(ComponentVersionMapMember("dstRect", sizeof(component->dstRect), &component->dstRect, "SDL_Rect"))
				->addMember(ComponentVersionMapMember("srcRect", sizeof(component->srcRect), &component->srcRect, "SDL_Rect"))
				->addMember(ComponentVersionMapMember("isVisible", sizeof(component->isVisible), &component->isVisible, "bool"))
				->addMember(ComponentVersionMapMember("offset", sizeof(component->offset), &component->offset, "Vec2"));

			return versionMap;
		};

		void Draw(SDL_Renderer* renderer, Vec2 transform);
		void UseTexture(SDL_Texture *texture);
	};
}
