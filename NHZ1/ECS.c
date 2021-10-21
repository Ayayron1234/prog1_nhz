#pragma warning(disable : 4996)
#include "ECS.h"

ComponentLists* ECS_init(int maxNumberOfComponents, bool doDeserialisation, char saveDirectory[255])
{
	int total_positionComponents = 0;
	Position* positionComponents = (Position*)calloc(maxNumberOfComponents, sizeof(Position));
	if (NULL == positionComponents) exit(1);

	if (doDeserialisation) {
		char path[255];

		strcpy(path, saveDirectory);
		Position_deserialise(positionComponents, &total_positionComponents, maxNumberOfComponents, strcat(path, "position.data"));
	}

	ComponentLists components = {
	.total_positionComponents = total_positionComponents,
	.positionComponents = positionComponents
	};
	return &components;
}

void ECS_deserialiseAllComponents(ComponentLists* components, char* directory)
{

}

void ECS_serialiseAllComponents(ComponentLists* components, char* directory)
{
}
