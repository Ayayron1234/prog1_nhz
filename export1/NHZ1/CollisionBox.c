#include "CollisionBox.h"

void CollisionBox_init(CollisionBox* collisionBoxes, int entityID, int* total_collisionBoxComponents, Vec2 size) {
	CollisionBox_delete(collisionBoxes, entityID, total_collisionBoxComponents);
	collisionBoxes[*total_collisionBoxComponents].ENTITY_ID = entityID;
	collisionBoxes[(*total_collisionBoxComponents)++].size = size;
}

void CollisionBox_delete(CollisionBox* collisionBoxes, int entityID, int* total_collisionBoxComponents) {
	bool found = false;
	for (int i = 0; i < *total_collisionBoxComponents; i++) {
		if (!found && collisionBoxes[i].ENTITY_ID == entityID)
			found = true;
		if (found)
			collisionBoxes[i] = (i < *total_collisionBoxComponents - 1) ? collisionBoxes[i + 1] : (CollisionBox) { 0 };
	}
	if (found) (*total_collisionBoxComponents)--;
}

void CollisionBox_deserialise(CollisionBox* collisionBoxes, int* total_collisionBoxComponents, int maxNumberOfComponents, char path[255]) {
	SDL_Log("Loading %d CollisionBox components from file: %s...", maxNumberOfComponents, path);
	CollisionBox* loadedTiles;
	loadedTiles = (CollisionBox*)calloc(maxNumberOfComponents, sizeof(CollisionBox));
	if (NULL == loadedTiles) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedTiles, sizeof(CollisionBox), maxNumberOfComponents, file);
		*total_collisionBoxComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedTiles[i].ENTITY_ID != 0)
			*total_collisionBoxComponents += 1;
		else break;
		collisionBoxes[i] = loadedTiles[i];
	}
	free(loadedTiles);
	if (file != 0) fclose(file);
}

void CollisionBox_serialise(CollisionBox* collisionBoxes, int maxNumberOfComponents, char path[255]) {
	SDL_Log("Saving %d CollisionBox components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(collisionBoxes, sizeof(CollisionBox), maxNumberOfComponents, file);
		fclose(file);
	}
}
