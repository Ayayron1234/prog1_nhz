#include "Game.h"
#include "Vec2.h"

int main(int argc, char* argv[])
{
	RGBAColor gameBackground = { .r = 85, .g = 140, .b = 250, .a = 255 };
	Vec2 gameDimentions = {720, 480};
	Game game = initGame("main window", gameDimentions, false, gameBackground);

	while (game.isRunning)
	{
		handleSDLEvents(&game);
		updateGame(&game);
		renderGameElements(&game);
	}

	quitGame(&game);
	return 0;
}
