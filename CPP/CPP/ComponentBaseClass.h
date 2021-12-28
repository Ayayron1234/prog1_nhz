#pragma once
#include <cstring>
#include <vector>
#include <iostream>


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
			if (index < 0 || index >= this->members.size()) throw std::out_of_range("ComponentVersionBluePrint: Index is out of range!");
			return &this->members.at(index);
		}
		ComponentVersionMapMember* operator[](const char* name) {
			for (int i = 0; i < this->members.size(); i++) {
				if (strcmp(this->members.at(i).name, name) == 0) return &this->members.at(i);
			}
			throw std::out_of_range("ComponentVersionBluePrint: Index is out of range!");
		}
	};
	std::ostream& operator<<(std::ostream& os, ComponentVersionMap& bluePrint);

	// when saving game data to a file, it is unnecessary to save entities which do not change throughout the game. 
	// to solve this, entities, or components should be tagged 
	//		my first ideo for a solution is the bellow enum
	//		STATIC means that it will never change without editing the map
	//		DYNAMIC means that it changes in the game

	// when loading in the game, it first neads to load the static data, then the dynamic, if a dynamic component is included in the game's initial state
	// it has to be overwritten by the dynamic savefile's data. 

	// this implementation would also allow me to change the initial game data, than load it to a game without losing progress. 

	// in short, a component is not saved if it's SAVE_TYPE is static. (when we are not editing the initial game data.)

	class Component {
	public:
		enum ComponenetSaveType { STATIC, DYNAMIC };
		
		int ENTITY_ID;
		ComponenetSaveType SAVE_TYPE;

		Component(int entity_id = 0) {
			this->ENTITY_ID = entity_id;
			this->SAVE_TYPE = STATIC;
		}

		ComponentVersionMap* getVersionMap();
	};
}
