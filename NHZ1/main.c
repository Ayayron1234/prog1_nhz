#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"


int main(void) {
	Game game;
	Game_init(&game, "test window", (Vec2Int) { 1080, 720 }, false, (RGBAColor) { .r = 85, .g = 140, .b = 250, .a = 255 });

	while (game.isRunning)
	{
		Game_handleSDLEvents(&game);
		Game_update(&game);
		Game_renderElements(&game);
	}

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

	free(game.components.positionComponents);
	free(game.components.spriteComponents);
	return 0;
}