#include "Position.h"

Position Position_init(Vec2 value)
{
	Position position = {.value = value};
	return position;
}

void Position_moveBy(Position* position, Vec2 amount)
{
	position->value.x += amount.x;
	position->value.y += amount.y;
}
