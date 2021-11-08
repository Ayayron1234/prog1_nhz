#include "PhysicsBody.h"

void PhysicsBody_init(PhysicsBody* physicsBodies, int entityID, int* total_physicsBodyComponents, double mass) {
	PhysicsBody_delete(physicsBodies, entityID, total_physicsBodyComponents);
	physicsBodies[*total_physicsBodyComponents].ENTITY_ID = entityID;
	physicsBodies[*total_physicsBodyComponents].mass = mass;
	physicsBodies[*total_physicsBodyComponents].gravitationalAcceleration = (Vec2){0,10};
	physicsBodies[*total_physicsBodyComponents].velocity = (Vec2){ 0,0 };
	physicsBodies[(*total_physicsBodyComponents)++].acceleration = (Vec2){ 0,0 };
}

void PhysicsBody_delete(PhysicsBody* physicsBodies, int entityID, int* total_physicsBodyComponents) {
	bool found = false;
	for (int i = 0; i < *total_physicsBodyComponents; i++) {
		if (!found && physicsBodies[i].ENTITY_ID == entityID)
			found = true;
		if (found)
			physicsBodies[i] = (i < *total_physicsBodyComponents - 1) ? physicsBodies[i + 1] : (PhysicsBody) { 0 };
	}
	if (found) (*total_physicsBodyComponents)--;
}

void PhysicsBody_deserialise(PhysicsBody* physicsBodies, int* total_physicsBodyComponents, int maxNumberOfComponents, char path[255]) {
	SDL_Log("Loading %d PhysicsBody components from file: %s...", maxNumberOfComponents, path);
	PhysicsBody* loadedPhysicsBodies;
	loadedPhysicsBodies = (PhysicsBody*)calloc(maxNumberOfComponents, sizeof(PhysicsBody));
	if (NULL == loadedPhysicsBodies) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedPhysicsBodies, sizeof(PhysicsBody), maxNumberOfComponents, file);
		*total_physicsBodyComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedPhysicsBodies[i].ENTITY_ID != 0)
			*total_physicsBodyComponents += 1;
		else break;
		physicsBodies[i] = loadedPhysicsBodies[i];
	}
	free(loadedPhysicsBodies);
	if (file != 0) fclose(file);
}

void PhysicsBody_serialise(PhysicsBody* physicsBodies, int maxNumberOfComponents, char path[255]) {
	SDL_Log("Saving %d PhysicsBody components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(physicsBodies, sizeof(PhysicsBody), maxNumberOfComponents, file);
		fclose(file);
	}
}