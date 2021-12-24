#ifndef POSITION_H
#define POSITION_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL.h>
#include "Math.h"


typedef struct Position {
	int ENTITY_ID;
	Vec2 value;
} Position;

// TODO: versioning.
// Bellow is a possible solution. 
typedef struct BluePrintMember {
	char name[32];
	size_t size;
	void* ptr;
	char format[32];
} BluePrintMember;

typedef struct BluePrint {
	int numberOfMembers;
	BluePrintMember* members;
} BluePrint;

void ECS_bluePringAddMember(BluePrint* bluePrint, BluePrintMember newMember);
BluePrint Position_getBluePrint(Position* position);

// temp

#endif // !POSITION_H

