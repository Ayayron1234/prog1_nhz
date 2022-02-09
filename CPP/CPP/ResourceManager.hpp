#pragma once
#include <memory>
#include "SDL_image.h"

namespace ECS {

	struct Tilemap {
		Vec2Int tileSize;
		SDL_Texture* texture;

		Tilemap(Vec2Int tileSize = Vec2Int(), SDL_Texture* texture = nullptr) : tileSize(tileSize), texture(texture) { }

		static Tilemap* LoadFromFile(std::string filepath, Vec2Int tileSize, SDL_Renderer* renderer) {
			Tilemap* tilemap = new Tilemap();
			tilemap->tileSize = tileSize;
			tilemap->texture = IMG_LoadTexture(renderer, filepath.c_str());
			if (tilemap->texture == nullptr) return nullptr;

			return tilemap;
		}

		~Tilemap() {
			if (this->texture != nullptr)
				SDL_DestroyTexture(this->texture);
		}
	};

	class Resource
	{
	private:
		void* data;
		size_t size;
		Resource* next;
		bool isLoaded;

	public:
		enum class ResourceType { STRING = 0, TILEMAP = 1, TEXTURE = 2, FONT_FAMILY = 3 };

		int RESOURCE_UID;
		ResourceType type;
	
		Resource(int resource_UID, ResourceType type, void* data)
			: data(data), RESOURCE_UID(resource_UID), type(type)
		{
			switch (type)
			{
			case ECS::Resource::ResourceType::STRING:
				this->size = strlen((char*)data);
				this->isLoaded = true;
				break;
			case ECS::Resource::ResourceType::TILEMAP:
				this->size = strlen((char*)data);
				this->isLoaded = false;
				break;
			case ECS::Resource::ResourceType::TEXTURE:
				this->size = strlen((char*)data);
				this->isLoaded = false;
				break;
			case ECS::Resource::ResourceType::FONT_FAMILY:
				// TODO: Handle creation of font family resource
				break;
			default:
				break;
			}
			this->size = size;
			this->next = nullptr;
		}

		void* Get() {
			if (this->isLoaded)
				return this->data;
			else throw;
		}

		Resource& operator=(Resource sourceRes) {
			this->data = std::malloc(sourceRes.size);
			this->size = sourceRes.size;
			std::memcpy(this->data, sourceRes.data, sourceRes.size);
			this->isLoaded = sourceRes.isLoaded;
			this->next = sourceRes.next;
			this->RESOURCE_UID = sourceRes.RESOURCE_UID;
			this->type = sourceRes.type;
		}

		friend class ResourceManager;
	};

	class ResourceReference {
	public:
		int RESOURCE_UID;
		Resource* ptr;

		ResourceReference(int resource_UID = 0, Resource* ptr = nullptr) {
			this->RESOURCE_UID = resource_UID;
			this->ptr = ptr;
		}
	};

	class ResourceManager {
	private:
		Resource* resourceHead;

		int lowestUnusedUID;

	public:
		ResourceManager() {
			this->resourceHead = nullptr;
			this->lowestUnusedUID = 1;
		}

		void RegisterResource(void* data, Resource::ResourceType type) {
			if (this->resourceHead == nullptr) return;

			Resource* nodePtr = this->resourceHead;
			while (nodePtr->next != nullptr) nodePtr = nodePtr->next;

			nodePtr->next = new Resource(lowestUnusedUID++, type, data);
		}

		Resource* GetResource(int resource_UID) {
			Resource* nodePtr = this->resourceHead;
		
			while (nodePtr != nullptr) {
				if (nodePtr->RESOURCE_UID == resource_UID) return nodePtr;
				nodePtr = nodePtr->next;
			}

			return nullptr;
		}

		void UpdateResource(int resource_UID, void* newData, size_t size) {
			Resource* resource = GetResource(resource_UID);
			if (resource == nullptr) return;
		
			if (resource->data != nullptr)
				std::free(resource->data);

			resource->data = newData;
			resource->size = size;
		}
	};

}
