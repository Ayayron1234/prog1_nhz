#include "Animation.h"


void Animation_init(Animation* animations, int entity_ID, int* total_animationComponents, Vec2Int tilePosition, int frameCount, double animationSpeed)
{
	Animation_delete(animations, entity_ID, total_animationComponents);
	animations[*total_animationComponents].ENTITY_ID = entity_ID;
	animations[*total_animationComponents].tilePosition = tilePosition;
	animations[*total_animationComponents].frameCount = frameCount;
	animations[(*total_animationComponents)++].animationSpeed = animationSpeed;
}

void Animation_delete(Animation* animations, int entity_ID, int* total_animationComponents) {
	bool found = false;
	for (int i = 0; i < *total_animationComponents; i++) {
		if (!found && animations[i].ENTITY_ID == entity_ID)
			found = true;
		if (found)
			animations[i] = (i < *total_animationComponents - 1) ? animations[i + 1] : (Animation) { 0 };
	}
	if (found) (*total_animationComponents)--;
}

void Animation_deserialise(Animation* animations, int* total_animationComponents, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Loading %d Animation components from file: %s...", maxNumberOfComponents, path);
	Animation* loadedAnimations;
	loadedAnimations = (Animation*)calloc(maxNumberOfComponents, sizeof(Animation));
	if (NULL == loadedAnimations) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedAnimations, sizeof(Animation), maxNumberOfComponents, file);
		*total_animationComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedAnimations[i].ENTITY_ID != 0)
			*total_animationComponents += 1;
		else break;
		animations[i] = loadedAnimations[i];
		animations[i].currentFrame = 0;
	}
	free(loadedAnimations);
	if (file != 0) fclose(file);
}

void Animation_serialise(Animation* animations, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Saving %d Animation components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(animations, sizeof(Animation), maxNumberOfComponents, file);
		fclose(file);
	}
}
