#include "Game.h"
#include "systems.h"


Game initGame(char* windowName, Vec2 windowDimentions, bool isFullScreen, RGBAColor backgroundColor)
{
	Game game = { .isRunning = true, .renderer = NULL, .t = {2, 2}, .backgroundColor = backgroundColor, .ENTITIES = 0 };
	int flags = SDL_WINDOW_RESIZABLE;
	flags = (isFullScreen) ? flags | SDL_WINDOW_FULLSCREEN : flags;

	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL cound not be initialised: %s\n", SDL_GetError());
		return;
	}
	// Create window
	game.window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowDimentions.x, windowDimentions.y, flags);
	if (game.window == NULL)
	{
		SDL_Log("SDL couldn't open %s window: %s\n", windowName, SDL_GetError());
		return;
	}
	// Create rendering context
	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_SOFTWARE);
	if (game.renderer == NULL)
	{
		SDL_Log("SDL couldn't create rendering context: %s", SDL_GetError());
		return;
	}
	SDL_RenderClear(game.renderer);

	int entity0 = ECS_createEntity(&game.ENTITIES);
	Texture texr = Texture_init(game.renderer, "image.jpg", true);
	ECS_attachTextureComponent(&game.components, entity0, &texr);
	ECS_attachPositionComponent(&game.components, entity0, &(Position) {.value = {50, 0}});

	int entity1 = ECS_createEntity(&game.ENTITIES);
	Texture texr2 = Texture_init(game.renderer, "idle_left.gif", true);
	ECS_attachTextureComponent(&game.components, entity1, &texr2);
	ECS_attachPositionComponent(&game.components, entity1, &(Position) {.value = { 150, 150 }});

	return game;
}

void quitGame(Game *game)
{
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);
	SDL_Quit();
}

void updateGame(Game *game)
{
	int t = updateGameTime(&game->t);
}

long int updateGameTime(ElapsedTime *gameTime)
{
	long int t = 0;
	if (!gameTime->start)
	{
		gameTime->start = (long int)time(NULL);
		gameTime->stop = gameTime->start + 1;
	}
	else
	{
		t = gameTime->stop - gameTime->start;
		gameTime->start = (long int)time(NULL);
	}

	return (t == 0) ? 1 : t;
}

void handleSDLEvents(Game *game)
{

	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		game->isRunning = false;
		break;
	default:
		break;
	}
}

void renderGameElements(Game *game)
{
	SDL_SetRenderDrawColor(game->renderer, game->backgroundColor.r, game->backgroundColor.g, game->backgroundColor.b, game->backgroundColor.a);
	SDL_RenderClear(game->renderer);

	for (int i = 0; i < game->components.total_textureComponents; i++) {
		Position* position = ECS_getPositionComponent(&game->components, game->components.textureComponents[i].ENTITY_ID);
		if (position != 0) 
			Texture_render(game->renderer, &game->components.textureComponents[i], position->value);
		else 
			Texture_render(game->renderer, &game->components.textureComponents[i], (Vec2) { 0, 0 });
	}

	SDL_RenderPresent(game->renderer);
}
