#include "PositionSystems.h"

void Position_moveBy(Position* position, Vec2 amount)
{
	if (position != NULL && position->ENTITY_ID != NULL && position->ENTITY_ID != 0) {
		position->value.x += amount.x;
		position->value.y += amount.y;
	}
}