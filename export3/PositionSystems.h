#ifndef POSITIONSYSTEMS_H
#define POSITIONSYSTEMS_H

#include "ECS.h"


/**
 * Moves entity by a given amount. 
 * @param position The position component who's value we want to change. 
 * @param amount A 2d vector by which the component should be transformed. 
 */
void Position_moveBy(Position* position, Vec2 amount);

#endif // !POSITION_H
