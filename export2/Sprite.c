#include "Sprite.h"

void Sprite_init(Sprite* dest, int entityID, int* total_spriteComponents, Tilemap* tilemap, Vec2Int tilePosition)
{
	Sprite_delete(dest, entityID, total_spriteComponents);
	dest[*total_spriteComponents].ENTITY_ID = entityID;
	dest[*total_spriteComponents].tilemap = tilemap;
	dest[*total_spriteComponents].tilePosition = tilePosition;
	dest[(*total_spriteComponents)++].renderProps = (RenderProperties){ .isVisible = true, .opacity = 1.0, .scale = 1.0 };
}

void Sprite_delete(Sprite* sprites, int entityID, int* total_spriteComponents) {
	bool found = false;
	for (int i = 0; i < *total_spriteComponents; i++) {
		if (!found && sprites[i].ENTITY_ID == entityID)
			found = true;
		if (found)
			sprites[i] = (i < *total_spriteComponents - 1) ? sprites[i + 1] : (Sprite) { 0 };
	}
	if (found) (*total_spriteComponents)--;
}

void Sprite_deserialise(Sprite* sprites, int* total_spriteComponents, int maxNumberOfComponents, char path[255], Tilemap *tilemap)
{
	SDL_Log("Loading %d Sprite components from file: %s...", maxNumberOfComponents, path);
	Sprite* loadedSprites;
	loadedSprites = (Sprite*)calloc(maxNumberOfComponents, sizeof(Sprite));
	if (NULL == loadedSprites) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedSprites, sizeof(Sprite), maxNumberOfComponents, file);
		*total_spriteComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedSprites[i].ENTITY_ID != 0)
			*total_spriteComponents += 1;
		else break;
		sprites[i] = loadedSprites[i];
		sprites[i].tilemap = tilemap;
	}
	free(loadedSprites);
	if (file != 0) fclose(file);
}

void Sprite_serialise(Sprite* sprites, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Saving %d Sprite components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(sprites, sizeof(Sprite), maxNumberOfComponents, file);
		fclose(file);
	}
}
