#include "PositionComponent.h"

PositionComponent::PositionComponent(int entity_id, Vec2 value)
{
	this->ENTITY_ID = entity_id;
	this->SAVE_TYPE = ECS::Component::STATIC;
	this->value = value;
}

ECS::ComponentVersionMap* PositionComponent::getVersionMap()
{
	ECS::ComponentVersionMap* bluePrint = new ECS::ComponentVersionMap(1);
	bluePrint
		->addMember(ECS::ComponentVersionMapMember("ENTITY_ID", sizeof(this->ENTITY_ID), &this->ENTITY_ID, "int"))
		->addMember(ECS::ComponentVersionMapMember("SAVE_TYPE", sizeof(this->SAVE_TYPE), &this->SAVE_TYPE, "SAVE_TYPE"))
		->addMember(ECS::ComponentVersionMapMember("value", sizeof(this->value), &this->value, "Vec2"));
	return bluePrint;
}