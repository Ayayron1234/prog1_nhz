#include "Position.h"

Position* Position_init(Position* dest, Vec2 value)
{
	*dest = (Position){ .value = (Vec2){.x = value.x, .y = value.y}, .ENTITY_ID = 0 };
	return dest;
}

void Position_deserialise(Position *position, int *total_PositionComponents, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Loading %d Position components from file: %s...", maxNumberOfComponents, path);
	Position* loadedPositions;
	loadedPositions = (Position*)calloc(maxNumberOfComponents, sizeof(struct Position));
	if (NULL == loadedPositions) exit(1);

	FILE* ifp;
	fopen_s(&ifp, path, "rb");
	if (NULL == ifp) exit(1);
	if (ifp != 0) {
		fread(loadedPositions, sizeof(struct Position), maxNumberOfComponents, ifp);
		*total_PositionComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		position[i] = loadedPositions[i];
		if (loadedPositions[i].ENTITY_ID != 0)
			*total_PositionComponents += 1;
		else break;
	}
	free(loadedPositions);
	if (ifp != 0) fclose(ifp);
}
void Position_serialise(Position* positions, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Saving %d Position components to file: %s...", maxNumberOfComponents, path);

	FILE* f;
	fopen_s(&f, path, "wb");
	if (NULL == f) exit(1);
	if (f != 0) {
		fwrite(positions, sizeof(struct Position), maxNumberOfComponents, f);
		fclose(f);
	}
}

void Position_moveBy(Position* position, Vec2 amount)
{
	position->value.x += amount.x;
	position->value.y += amount.y;
}
