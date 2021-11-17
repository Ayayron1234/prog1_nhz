#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"


int main(void) {
	Game game;
	Game_init(&game, "test window", (Vec2Int) { 1320, 720 }, false, (RGBAColor) { .r = 85, .g = 140, .b = 250, .a = 255 });


	// ---------------------- fps ----------------------
#define MAXSAMPLES 500
	int tickindex = 0;
	int ticksum = 0;
	int ticklist[MAXSAMPLES] = { 0 };
	int lastUpdateTime = SDL_GetTicks();
	// -------------------------------------------------

	while (game.isRunning)
	{
		// ---------------------- fps ----------------------
		int newtick = SDL_GetTicks();
		ticksum -= ticklist[tickindex];
		ticksum += newtick - lastUpdateTime;
		ticklist[tickindex] = newtick - lastUpdateTime;
		tickindex = (tickindex + 1) % MAXSAMPLES;
		sprintf_s(((Text*)ECS_getComponent(TEXT, *game.currentLayout, 22))->value,
			255, "FPS: %.0f", 1000.0 / ((double)ticksum / MAXSAMPLES));
		lastUpdateTime = newtick;
		// -------------------------------------------------

		Game_handleSDLEvents(&game);
		Game_update(&game);
		Game_renderElements(&game);

	}

	//ECS_serialise(2, (ComponentLists[2]) { game.components, game.components });

	//Sprite spr[128];
	//for (int i = 0; i < 128; i++) {
	//	spr[i] = game.components.spriteComponents[i];
	//}
	//Position pos[128];
	//for (int i = 0; i < 128; i++) {
	//	pos[i] = game.components.positionComponents[i];
	//}
	//Editor edt[128];
	//for (int i = 0; i < 128; i++) {
	//	edt[i] = game.components.editorComponents[i];
	//}
	//Animation anim[128];
	//for (int i = 0; i < 128; i++) {
	//	anim[i] = game.components.animationComponents[i];
	//}
	//Tile tile[128];
	//for (int i = 0; i < 128; i++) {
	//	tile[i] = game.components.tileComponents[i];
	//}
	//Text text[128];
	//for (int i = 0; i < 128; i++) {
	//	text[i] = game.components.textComponents[i];
	//}
	//CollisionBox clsbx[128];
	//for (int i = 0; i < 128; i++) {
	//	clsbx[i] = game.components.collisionBoxComponents[i];
	//}
	//PhysicsBody phisicsBdy[128];
	//for (int i = 0; i < 128; i++) {
	//	phisicsBdy[i] = game.components.physicsBodyComponents[i];
	//}

	SerialisationMapFragment* serialisationMapFragments = ECS_serialise(game.layouts, 1);

	SerialisationMapFragment test[NUMBER_OF_COMPONENT_TYPES];
	for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++) {
		test[i] = serialisationMapFragments[i];
	}

	FILE* f;
	fopen_s(&f, "./saves/aaron.data", "wb");
	if (NULL == f) exit(1);
	int numberOfComponents = NUMBER_OF_COMPONENT_TYPES;
	fwrite(&numberOfComponents, sizeof(int), 1, f);
	fclose(f);

	fopen_s(&f, "./saves/aaron.data", "ab");
	if (NULL == f) exit(1);
	fwrite(&game.numberOfLayouts, sizeof(int), 1, f);
	for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++) {
		fwrite(&serialisationMapFragments[i].componentSize, sizeof(size_t), 1, f);
		fwrite(&serialisationMapFragments[i].componentType, sizeof(int), 1, f);
		fwrite(serialisationMapFragments[i].layoutMaps, sizeof(LayoutMap), game.numberOfLayouts, f);
		fwrite(&serialisationMapFragments[i].total_components, sizeof(int), 1, f);
		fwrite(&serialisationMapFragments[i].total_layouts, sizeof(int), 1, f);
	}	
	for (int componentTypeIndex = 0; componentTypeIndex < NUMBER_OF_COMPONENT_TYPES; componentTypeIndex++) {
		fwrite(game.componentLists[componentTypeIndex], ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL), serialisationMapFragments[componentTypeIndex].total_components, f);
		//fwrite(game.layouts[0].componentListsPointers[i], ECS_getSizeAndTypeOfComponent(i, NULL), serialisationMapFragments[i].total_components, f);
	}

	fclose(f);


	//fopen_s(&f, "./saves/aaron.data", "rb");
	//if (NULL == f) exit(1);
	//SerialisationMapFragment* loaded;	

	//int numberOfLoadedComponentTypes;
	//fread(&numberOfLoadedComponentTypes, sizeof(int), 1, f);
	//int numberOfLoadedLayouts;
	//fread(&numberOfLoadedLayouts, sizeof(int), 1, f);

	//loaded = (SerialisationMapFragment*)malloc(numberOfLoadedComponentTypes * sizeof(SerialisationMapFragment));
	//for (int i = 0; i < numberOfLoadedComponentTypes; i++) {
	//	loaded[i].layoutMaps = (LayoutMap*)malloc(numberOfLoadedLayouts * sizeof(LayoutMap));
	//	fread(&loaded[i].componentSize, sizeof(size_t), 1, f);
	//	fread(&loaded[i].componentType, sizeof(int), 1, f);
	//	fread(loaded[i].layoutMaps, sizeof(LayoutMap), game.numberOfLayouts, f);
	//	fread(&loaded[i].total_components, sizeof(int), 1, f);
	//	fread(&loaded[i].total_layouts, sizeof(int), 1, f);
	//}

	//game.componentLists = (void**)malloc(numberOfLoadedComponentTypes * sizeof(void*));
	//for (int componentTypeIndex = 0; componentTypeIndex < numberOfLoadedComponentTypes; componentTypeIndex++) {
	//	//game.layouts->componentListsPointers[componentTypeIndex] = calloc(loaded[componentTypeIndex].total_components, ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL));
	//	game.componentLists[componentTypeIndex] = 
	//		malloc(loaded[componentTypeIndex].total_components * ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL));
	//	fread(game.componentLists[componentTypeIndex], ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL), loaded[componentTypeIndex].total_components, f);
	//	for (int i = 0; i < numberOfLoadedLayouts; i++) {
	//		game.layouts[i].componentListsPointers[componentTypeIndex] = game.componentLists[componentTypeIndex];
	//	}
	//	//fread(game.layouts->componentListsPointers[componentTypeIndex], sizeof(ECS_getSizeAndTypeOfComponent(componentTypeIndex, NULL)), loaded[componentTypeIndex].total_components, f);
	//}
	//fclose(f);

	//Position test3[50];
	//for (int i = 0; i < loaded[POSITION].total_components; i++) {
	//	test3[i] = *((Position*)game.layouts->componentListsPointers[POSITION] + i);
	//}

	//SerialisationMapFragment test2[NUMBER_OF_COMPONENT_TYPES];
	//for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++) {
	//	test2[i] = loaded[i];
	//}

	//free(game.components.positionComponents);
	//free(game.components.spriteComponents);
	//free(game.components.editorComponents);
	//free(game.components.animationComponents);
	//free(game.components.tileComponents);
	//free(game.components.textComponents);
	//free(game.components.collisionBoxComponents);
	//free(game.components.colliderComponents);
	//free(game.components.physicsBodyComponents);
	SDL_DestroyTexture(game.tilemap.texture);

	SDL_Quit();
	TTF_Quit();

	debugmalloc_log_file("./debugmalloc.log");
	return 0;
}