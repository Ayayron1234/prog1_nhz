#include "Tile.h"

void Tile_init(Tile* tiles, int entityID, int* total_tileComponents, Tilemap* tilemap, Vec2Int tilePosition, Vec2Int size) {
	Tile_delete(tiles, entityID, total_tileComponents);
	tiles[*total_tileComponents].ENTITY_ID = entityID;
	tiles[*total_tileComponents].tilemap = tilemap;
	tiles[*total_tileComponents].tilePosition = tilePosition;
	tiles[*total_tileComponents].size = size;
	tiles[(*total_tileComponents)++].renderProps = (RenderProperties){ .isVisible = true, .opacity = 1.0, .scale = 1.0 };
}

void Tile_delete(Tile* tiles, int entityID, int* total_tileComponents) {
	bool found = false;
	for (int i = 0; i < *total_tileComponents; i++) {
		if (!found && tiles[i].ENTITY_ID == entityID)
			found = true;
		if (found)
			tiles[i] = (i < *total_tileComponents - 1) ? tiles[i + 1] : (Tile) { 0 };
	}
	if (found) (*total_tileComponents)--;
}

void Tile_deserialise(Tile* tiles, int* total_tileComponents, int maxNumberOfComponents, char path[255], Tilemap *tilemap) {
	SDL_Log("Loading %d Tile components from file: %s...", maxNumberOfComponents, path);
	Tile* loadedTiles;
	loadedTiles = (Tile*)calloc(maxNumberOfComponents, sizeof(Tile));
	if (NULL == loadedTiles) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedTiles, sizeof(Tile), maxNumberOfComponents, file);
		*total_tileComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedTiles[i].ENTITY_ID != 0)
			*total_tileComponents += 1;
		else break;
		tiles[i] = loadedTiles[i];
		tiles[i].tilemap = tilemap;
	}
	free(loadedTiles);
	if (file != 0) fclose(file);
}

void Tile_serialise(Tile* tiles, int maxNumberOfComponents, char path[255]) {
	SDL_Log("Saving %d Tile components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(tiles, sizeof(Tile), maxNumberOfComponents, file);
		fclose(file);
	}
}
