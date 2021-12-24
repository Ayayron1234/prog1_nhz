#include "Position.h"

void ECS_bluePringAddMember(BluePrint* bluePrint, BluePrintMember newMember) {
	BluePrintMember* newMembers = (BluePrintMember*)calloc(++bluePrint->numberOfMembers, sizeof(BluePrintMember));
	if (newMembers == NULL) return;

	if (bluePrint->numberOfMembers > 0) {
		memcpy(newMembers, bluePrint->members, (bluePrint->numberOfMembers - 1) * sizeof(BluePrintMember));
		free(bluePrint->members);
	}

	newMembers[bluePrint->numberOfMembers - 1] = newMember;

	bluePrint->members = newMembers;
}

BluePrint Position_getBluePrint(Position* position) {
	BluePrint bluePrint = { 0 };

	ECS_bluePringAddMember(&bluePrint, (BluePrintMember) { .name = "ENTITY_ID", .size = sizeof(int), .ptr = &position->ENTITY_ID, .format = "%d" });
	ECS_bluePringAddMember(&bluePrint, (BluePrintMember) { .name = "value", .size = sizeof(Vec2), .ptr = &position->value, .format = "Vec2(%g, %g)" });

	return bluePrint;
}

