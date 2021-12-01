#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"


int main(void) {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
		sprintf(((Text*)ECS_getComponent(TEXT, *game.currentLayout, 22))->value, "FPS: %.0f", 1000.0 / ((double)ticksum / MAXSAMPLES));
		lastUpdateTime = newtick;
		// -------------------------------------------------

		Game_handleSDLEvents(&game);
		Game_update(&game);
		Game_renderElements(&game);

	}

	if (game.state != VICTORY)
		ECS_save(game.layouts, game.numberOfLayouts, game.player);

	SDL_DestroyTexture(game.resources.tilemap.texture);
	SDL_Quit();
	TTF_Quit();

	ECS_freeData(game.layouts, game.numberOfLayouts);
	return 0;
}