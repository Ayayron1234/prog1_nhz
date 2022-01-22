#pragma once

#include "ComponentBaseClass.h"
#include "Vec2.h"

class PositionComponent : public ECS::Component {
public:
	Vec2 value;
	PositionComponent(int entity_id, Vec2 value = Vec2(0, 0));

	ECS::ComponentVersionMap* getVersionMap();
};

template <> struct ECS::ComponentTypeSelect<ECS::POSITION_COMPONENT> {
	typedef PositionComponent Type;
};
