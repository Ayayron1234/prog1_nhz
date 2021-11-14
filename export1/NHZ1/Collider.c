#include "Collider.h"

void Collider_init(Collider* colliders, int entityID, int* total_colliderComponents, bool isDynamic) {
	Collider_delete(colliders, entityID, total_colliderComponents);
	colliders[*total_colliderComponents].ENTITY_ID = entityID;
	colliders[(*total_colliderComponents)++].type = isDynamic ? DYNAMIC : STATIC;
}

void Collider_delete(Collider* colliders, int entityID, int* total_colliderComponents) {
	bool found = false;
	for (int i = 0; i < *total_colliderComponents; i++) {
		if (!found && colliders[i].ENTITY_ID == entityID)
			found = true;
		if (found)
			colliders[i] = (i < *total_colliderComponents - 1) ? colliders[i + 1] : (Collider) { 0 };
	}
	if (found) (*total_colliderComponents)--;
}

void Collider_deserialise(Collider* colliders, int* total_colliderComponents, int maxNumberOfComponents, char path[255]) {
	SDL_Log("Loading %d Collider components from file: %s...", maxNumberOfComponents, path);
	Collider* loadedTiles;
	loadedTiles = (Collider*)calloc(maxNumberOfComponents, sizeof(Collider));
	if (NULL == loadedTiles) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedTiles, sizeof(Collider), maxNumberOfComponents, file);
		*total_colliderComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedTiles[i].ENTITY_ID != 0)
			*total_colliderComponents += 1;
		else break;
		colliders[i] = loadedTiles[i];
	}
	free(loadedTiles);
	if (file != 0) fclose(file);
}

void Collider_serialise(Collider* colliders, int maxNumberOfComponents, char path[255]) {
	SDL_Log("Saving %d Collider components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(colliders, sizeof(Collider), maxNumberOfComponents, file);
		fclose(file);
	}
}
