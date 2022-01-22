#include "ComponentBaseClass.h"
#include "Vec2.h"
#include "SDL.h"

using namespace ECS;

struct Matrix_2x2
{
	double _00;
	double _01;
	double _10;
	double _11;
};

ComponentVersionMap* ComponentVersionMap::addMember(ComponentVersionMapMember member) {
	this->members.push_back(member);
	this->componentSize += member.size;
	return this;
}

ECS::ComponentVersionMapMember::ComponentVersionMapMember(const char name[32], size_t size, void* ptr, const char format[32])
{
	strcpy_s(this->name, name);
	this->size = size;
	this->ptr = ptr;
	strcpy_s(this->format, format);
}

std::ostream& ECS::operator<<(std::ostream& os, ComponentVersionMapMember& member)
{
	os << "   \033[1m\033[34m" << member.format << "\033[0m " << member.name << "\t{size: " << member.size << ", currentValue: ";
	if (strcmp(member.format, "int") == 0) os << *((int*)member.ptr);
	else if (strcmp(member.format, "double") == 0) os << *((double*)member.ptr);
	else if (strcmp(member.format, "char") == 0) os << "'" << *((char*)member.ptr) << "'";
	else if (strcmp(member.format, "char[]") == 0) {
		os << "\"";
		for (int i = 0; i < strlen((char*)member.ptr); i++)
			os << ((char*)member.ptr)[i];
		os << "\"";
	}
	else if (strcmp(member.format, "bool") == 0) os << *((bool*)member.ptr);
	else if (strcmp(member.format, "ptr") == 0) os << member.ptr;
	else if (strcmp(member.format, "Vec2") == 0) os << *((Vec2*)member.ptr);
	else if (strcmp(member.format, "Vec2Int") == 0) os << *((Vec2Int*)member.ptr);
	else if (strcmp(member.format, "Matrix_2x2") == 0) {
		Matrix_2x2 M = *(Matrix_2x2*)member.ptr;
		os << "Matrix_2x2{ {" << M._00 << ", " << M._01 << "}, {" << M._10 << ", " << M._11 << "} }";
	}
	else if (strcmp(member.format, "SAVE_TYPE") == 0)
		switch (*((Component::ComponenetSaveType*)member.ptr))
		{
		case Component::ComponenetSaveType::STATIC: os << "STATIC (0)"; break;
		case Component::ComponenetSaveType::DYNAMIC: os << "DYNAMIC (1)"; break;
		default: break;
		}
	else if (strcmp(member.format, "SDL_Rect") == 0) os << "SDL_Rect(x=" << (*(SDL_Rect*)member.ptr).x << ", y=" << (*(SDL_Rect*)member.ptr).y << ", w=" << (*(SDL_Rect*)member.ptr).w << ", h=" << (*(SDL_Rect*)member.ptr).h << ")";
	os << "}";
	return os;
}

ComponentVersionMapMember ComponentVersionMapMember::create(const char name[32], size_t size, void* ptr, const char format[32])
{
	ComponentVersionMapMember member(name, size, ptr, format);
	return member;
}

std::ostream& ECS::operator<<(std::ostream& os, ComponentVersionMap& bluePrint) {
	os << "\n\033[1m\033[37mBluePrint\033[0m#\033[1m\033[33m" << bluePrint.componentType << "\033[0m\n{\n";
	for (ComponentVersionMapMember member : bluePrint.members) {
		os << member << std::endl;
	}
	os << "}\n";
	return os;
}

ComponentVersionMap* Component::getVersionMap(Component* component) {
	if (component == nullptr) component = this;

	ComponentVersionMap* versionMap = new ComponentVersionMap(0);
	versionMap
		->addMember(ComponentVersionMapMember::create("ENTITY_ID", sizeof(component->ENTITY_ID), &component->ENTITY_ID, "int"))
		->addMember(ComponentVersionMapMember::create("SAVE_TYPE", sizeof(component->SAVE_TYPE), &component->SAVE_TYPE, "SAVE_TYPE"));
	return versionMap;
}

