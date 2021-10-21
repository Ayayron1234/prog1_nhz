#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"


int main(void) {

	Game game = Game_init("test window", (Vec2) { 720, 480 }, false, (RGBAColor) { .r = 85, .g = 140, .b = 250, .a = 255 });
	
	printf("%d", game.ENTITIES);

	return 0;
}