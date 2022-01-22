#pragma once
#include "VisualComponent.h"
#include "ResourceManager.hpp"

namespace ECS {

	class Tile : public VisualComponent {
	private:
		ResourceReference tilemapReference;
	public:
		Vec2 size;
		Vec2Int tilePosition;

		Tile(ResourceReference tilemapReferenceUID, int entity_id = 0, Vec2Int tilePosition = Vec2Int(), Vec2 size = Vec2()) {
			this->tilemapReference = tilemapReference;

			this->ENTITY_ID = entity_id;
			this->tilePosition = tilePosition;
			this->size = size;
		}

		ComponentVersionMap* getVersionMap() {
			ComponentVersionMap* versionMap = VisualComponent::getVersionMap(this);
			versionMap->componentType = ECS::TILE_COMPONENT;

			versionMap
				->addMember(ComponentVersionMapMember("size", sizeof(this->size), &this->size, "Vec2"))
				->addMember(ComponentVersionMapMember("tilemapReference", sizeof(this->tilemapReference), &this->tilemapReference, "ResourceReference"))
				->addMember(ComponentVersionMapMember("tilePosition", sizeof(this->tilePosition), &this->tilePosition, "Vec2Int"));

			return versionMap;
		};
	};

	
	template <> struct ECS::ComponentTypeSelect<ECS::TILE_COMPONENT> {
		typedef Tile Type;
	};

}