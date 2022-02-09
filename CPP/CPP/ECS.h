#pragma once
#include "ComponentBaseClass.h"
#include "VisualComponent.h"
#include "ComponentVersionMap.h"
#include <string>
#include <vector>

namespace ECS {

	class EntityManager {
	public:
		struct ComponentList {
		private:
			void* list;
			ComponentVersionMap* versionMap;

		public:
			int length;
			size_t componentSize() { return this->versionMap->componentSize; }

			ComponentList(int length = 0, ComponentVersionMap* versionMap = nullptr) {
				this->length = length;
				this->versionMap = versionMap;
				//if (this->versionMap != nullptr)
				//	this->list = operator new[](this->length * this->versionMap->componentSize);
				//else 
					this->list = nullptr;
			}

			template <int componentType> typename ComponentTypeSelect<componentType>::Type* at(int index) {
				if (index >= this->length || index < 0) {
					std::string errMsg;
					errMsg += "ComponentList: Index: ";
					errMsg += index;
					errMsg += "is out of range. ";
					throw std::out_of_range(errMsg);
				}
				return &((typename ComponentTypeSelect<componentType>::Type*)this->list)[index];
			}
			Component* at(int index) {
				return (Component*)((char*)this->list + index * this->versionMap->componentSize);
			}

			void alloc() {
				if (this->versionMap != nullptr && this->length > 0)
					this->list = std::calloc(this->length, this->versionMap->componentSize);
					//this->list = operator new[](this->length * this->versionMap->componentSize);
			}

			template <int componentType> typename ComponentTypeSelect<componentType>::Type* GetFirstUnusedSpot() {
				for (int i = 0; i < this->length; i++) {
					typename ComponentTypeSelect<componentType>::Type* ptr = this->at<componentType>(i);
					if (ptr->ENTITY_ID == 0) return ptr;
				}
				throw std::out_of_range("ComponentList: Maximum number of components reached. ");
			}

			//void operator=(ComponentList source) {
			//	this->length = source.length;
			//	this->versionMap = source.versionMap;

			//	if (this->list == nullptr)
			//	this->list = std::calloc(this->length, this->versionMap->componentSize);
			//	
			//	if (this->list == NULL) throw;
			//	std::memcpy(this->list, source.list, this->length * this->versionMap->componentSize);
			//}

			~ComponentList() {
				if (this->length > 0 && this->list != nullptr) {
					std::free(this->list);
					//operator delete[](this->list);
					this->list = nullptr;
				}
			}
		};

	private:
		int _maxNumberOfComponents;
		ComponentList componentLists[NUMBER_OF_COMPONENT_TYPES];

	public:

		EntityManager(ComponentVersionMap* componentVersionMaps[NUMBER_OF_COMPONENT_TYPES], const int maxNumberOfComponents = 500) {
			this->_maxNumberOfComponents = maxNumberOfComponents;
			for (int componentTypeIndex = 0; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) {
				this->componentLists[componentTypeIndex] = ComponentList(this->_maxNumberOfComponents, componentVersionMaps[componentTypeIndex]);
				this->componentLists[componentTypeIndex].alloc();
			}
		}

		template <int componentType> typename ComponentTypeSelect<componentType>::Type* CreateComponent(typename ComponentTypeSelect<componentType>::Type component) {
			typename ComponentTypeSelect<componentType>::Type* ptr = this->componentLists[componentType].GetFirstUnusedSpot<componentType>();
			
			*ptr = component;
			return ptr;
		}

		ComponentList* GetComponentList(ComponentType componentType) {
			return &this->componentLists[componentType];
		}
	};
}
