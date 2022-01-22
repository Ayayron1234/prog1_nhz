#pragma once

namespace ECS {

	class ComponentVersionMapMember {
	public:
		char name[32];
		size_t size;
		void* ptr;
		char format[32];

		ComponentVersionMapMember(const char name[32], size_t size, void* ptr, const char format[32]);
		static ComponentVersionMapMember create(const char name[32], size_t size, void* ptr, const char format[32]);
	};
	std::ostream& operator<<(std::ostream& os, ComponentVersionMapMember& member);

	class ComponentVersionMap {
	public:
		//static ComponentVersionMap versionMapsOfLoadedComponents[NUMBER_OF_COMPONENT_TYPES];
		//static void StoreVersionMap(ComponentType componentType, ComponentVersionMap versionMap) {
		//	ComponentVersionMap::versionMapsOfLoadedComponents[componentType] = ComponentVersionMap(versionMap);
		//}

		int componentType;
		size_t componentSize;
		std::vector<ComponentVersionMapMember> members;

		ComponentVersionMap(int componentType = 0) {
			this->componentType = componentType;
			this->members;
			this->componentSize = 0;
		};

		ComponentVersionMap* addMember(ComponentVersionMapMember member);

		ComponentVersionMapMember* operator[](int index) {
			if (index < 0 || index >= this->members.size()) throw std::out_of_range("ComponentVersionMapMember: Index is out of range!");
			return &this->members.at(index);
		}
		ComponentVersionMapMember* operator[](const char* name) {
			for (int i = 0; i < this->members.size(); i++) {
				if (strcmp(this->members.at(i).name, name) == 0) return &this->members.at(i);
			}
			throw std::out_of_range("ComponentVersionMapMember: Index is out of range!");
		}
	};
	std::ostream& operator<<(std::ostream& os, ComponentVersionMap& bluePrint);

}
