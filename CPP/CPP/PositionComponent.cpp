#include "PositionComponent.h"

PositionComponent::PositionComponent(int entity_id, Vec2 value)
{
	this->ENTITY_ID = entity_id;
	this->SAVE_TYPE = ECS::Component::ComponenetSaveType::STATIC;
	this->value = value;
}

ECS::ComponentVersionMap* PositionComponent::getVersionMap()
{
	ECS::ComponentVersionMap* versionMap = Component::getVersionMap(this);
	versionMap->componentType = 1;
	versionMap
		->addMember(ECS::ComponentVersionMapMember("value", sizeof(this->value), &this->value, "Vec2"));
	return versionMap;
}