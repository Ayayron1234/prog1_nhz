#include "ComponentBaseClass.h"
#include "Vec2.h"

using namespace ECS;


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
	else if (strcmp(member.format, "Vec2") == 0) os << *((Vec2*)member.ptr);
	else if (strcmp(member.format, "SAVE_TYPE") == 0) 
		switch (*((Component::ComponenetSaveType*)member.ptr))
		{
		case Component::STATIC: os << "STATIC (0)"; break;
		case Component::DYNAMIC: os << "DYNAMIC (1)"; break;
		default: break;
		}
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

ComponentVersionMap* Component::getVersionMap() {
	ComponentVersionMap* bluePrint = new ComponentVersionMap(0);
	bluePrint
		->addMember(ComponentVersionMapMember::create("ENTITY_ID", sizeof(int), &this->ENTITY_ID, "int"));
	return bluePrint;
}

