#ifndef COLLISIONBOXSYSTEMS_H
#define COLLISIONBOXSYSTEMS_H

#include <stdbool.h>
#include "SDL.h"
#include "ECS.h"
#include "EditorSystems.h"
#include "Math.h"


/**
This struct is used for collision detection.
 */
typedef struct Ray2 {
	Vec2 origin;			///< origin point of the ray. 
	Vec2 destination;		///< destination point of the ray. 
} Ray2;


/**
 * Update size of collision box components.Gets called in each frame.
 */
void CollisionBox_update(int gameState, Layout* currentLayout, CollisionBox* collisionBox);

/**
 * Checks if a point is inside the collision box.
 * @param currentLayout The collision box's parent layout.
 * @param collisionBox The collision box which we want to check the point against. 
 * @param point The point we want to check. 
 * @returns logic true if the point is inside, false if it is outside.
 */
bool CollisionBox_isPointInside(Layout* currentLayout, CollisionBox* collisionBox, Vec2 point);

/**
 * Checks if two rectangles boxs overlap.
 * @pararm A An SDL_Rect rectangle.
 * @pararm B An SDL_Rect rectangle.
 * @returns logic true if they overlap, false if they don't. 
 */
bool CollisionBox_checkForOverlapp(SDL_Rect A, SDL_Rect B);

/**
 * Checks if ray overlapps with collision box. This is used in collision detection.
 * @param currentLayout The collision box's parent layout.
 * @param collisionBox The collision box which we want to check the point against. 
 * @param ray The ray we want to check. 
 * @param contactPoint A pointer to the 2d vector where we want to store the contact point of the ray and the collision box. 
 * @param contactNormal A pointer to the 2d vector where we want to store the normal vector of the collision.
 * @param tHitNear A double representing where the collision happened on teh ray. If the value is between 0 and 1, than there was a collision.
 * @returns logic true if they collide, false if they don't.
 */
bool CollisionBox_checkIfRayOverlapps(Layout* currentLayout, CollisionBox* collisionBox, Ray2 ray, Vec2* contactPoint, Vec2* contactNormal, double* tHitNear);

#endif // !COLLISIONBOXSYSTEMS_H
