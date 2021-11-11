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
	int ticklist[MAXSAMPLES] = {0};
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
		sprintf_s(ECS_getTextComponent(&game.components, 22)->value,
			255, "FPS: %.0f", 1000.0 / ((double)ticksum / MAXSAMPLES));
		lastUpdateTime = newtick;
		// -------------------------------------------------

		Game_handleSDLEvents(&game);
		Game_update(&game);
		Game_renderElements(&game);

	}

	ECS_serialise(2, (ComponentLists[2]) { game.components, game.components });

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

	free(game.components.positionComponents);
	free(game.components.spriteComponents);
	free(game.components.editorComponents);
	free(game.components.animationComponents);
	free(game.components.tileComponents);
	free(game.components.textComponents);
	free(game.components.collisionBoxComponents);
	free(game.components.colliderComponents);
	free(game.components.physicsBodyComponents);
	SDL_DestroyTexture(game.tilemap.texture);

	SDL_Quit();
	TTF_Quit();
	return 0;
}