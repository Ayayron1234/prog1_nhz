#pragma once
#include <cstring>
#include <vector>
#include <iostream>
#include "ComponentVersionMap.h"

#include <SDL.h>
#undef main

namespace ECS {

	const static int NUMBER_OF_COMPONENT_TYPES = 3;
	enum ComponentType {
		POSITION_COMPONENT = 0,
		TILE_COMPONENT = 1,
		TRANSFORM_COMPONENT = 2,
	};

	template <int n>
	struct ComponentTypeSelect { };
	
	class Component {
	public:
		// when saving game data to a file, it is unnecessary to save entities which do not change throughout the game. 
		// to solve this, entities, or components should be tagged 
		//		my first ideo for a solution is the bellow enum
		//		STATIC means that it will never change without editing the map
		//		DYNAMIC means that it changes in the game

		// when loading in the game, it first neads to load the static data, then the dynamic, if a dynamic component is included in the game's initial state
		// it has to be overwritten by the dynamic savefile's data. 

		// this implementation would also allow me to change the initial game data, then load it to a game without losing progress. 

		// in short, a component is not saved if it's SAVE_TYPE is static. (unless we are editing the initial game data.)
		enum class ComponenetSaveType { STATIC, DYNAMIC };
		
		int ENTITY_ID;
		ComponenetSaveType SAVE_TYPE;

		Component(int entity_id = 0) {
			this->ENTITY_ID = entity_id;
			this->SAVE_TYPE = ComponenetSaveType::STATIC;
		}

		ComponentVersionMap* getVersionMap(Component* component = nullptr);
		template <int componentType> typename ComponentTypeSelect<componentType>::Type* cast() { return (typename ComponentTypeSelect<componentType>::Type*)this; }
	};
}
