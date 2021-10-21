#include "Game.h"


Game Game_init(char* windowName, Vec2 windowDimentions, bool isFullScreen, RGBAColor backgroundColor)
{
	Game game = { .ENTITIES = 1, .backgroundColor = backgroundColor, .components = ECS_init(128, false, "./saves/") };

	for (int i = 0; i < 10; i++) 
		game.components->positionComponents[i] = (Position){ .value = (Vec2){5.0, 5.0}, .ENTITY_ID = ++game.components->total_positionComponents };

	//Position_serialise(game.components->positionComponents, 128, "./saves/position.data");
	Position_deserialise(game.components->positionComponents, &game.components->total_positionComponents, 128, "./saves/position.data");

	printf("%d", game.components->positionComponents[2].ENTITY_ID);
	

	int* a;
	a = (int*)calloc(2, sizeof(int));
	if (NULL == a) exit(1);
	a[1] = 2;

	free(a);

	//free(game.components->positionComponents);
	return game;
}
