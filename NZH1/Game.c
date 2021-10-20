#include <stdlib.h>
#include "Game.h"
#include "systems.h"


void ps(Game* game) {
	int a = 8;
	return;
}


Game initGame(char* windowName, Vec2 windowDimentions, bool isFullScreen, RGBAColor backgroundColor)
{
	Game game = { .isLoaded = false, .isRunning = true, .renderer = NULL, .backgroundColor = backgroundColor, .ENTITIES = 1, .components = {
		.positionComponents = (Position*) malloc(255 * sizeof(struct Position)),
		.textureComponents = (Texture*) malloc(255 * sizeof(struct Texture))
	} };

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
	Texture texr = Texture_init(game.renderer, "shroom.png", true);
	ECS_attachTextureComponent(&game.components, entity0, &texr);

	int entity1 = ECS_createEntity(&game.ENTITIES);
	Texture texr1 = Texture_init(game.renderer, "uncovered_tile.png", true);
	ECS_attachTextureComponent(&game.components, entity1, &texr1);

	int entity2 = ECS_createEntity(&game.ENTITIES);
	Texture texr2 = Texture_init(game.renderer, "suprise_tile.png", true);
	ECS_attachTextureComponent(&game.components, entity2, &texr2);

	int entity3 = ECS_createEntity(&game.ENTITIES);
	Texture texr3 = Texture_init(game.renderer, "ground_tile.png", true);
	ECS_attachTextureComponent(&game.components, entity3, &texr3);

	int entity4 = ECS_createEntity(&game.ENTITIES);
	ECS_attachTextureComponent(&game.components, entity4, &texr3);
	ECS_attachPositionComponent(&game.components, entity0, &(Position) {.value = {128, 128}});
	ECS_attachPositionComponent(&game.components, entity1, &(Position) {.value = { 128, 176 }});
	ECS_attachPositionComponent(&game.components, entity2, &(Position) {.value = { 176, 176 }});
	ECS_attachPositionComponent(&game.components, entity3, &(Position) {.value = { 128, 320 }});
	ECS_attachPositionComponent(&game.components, entity4, &(Position) {.value = { 176, 320 }});

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

}

void handleSDLEvents(Game *game)
{
	if (game->isLoaded == false) {
		ECS_deserialiseAllComponents(&game->components, "./saves/");
		game->isLoaded = true;
	}

	SDL_Event event;
	SDL_PollEvent(&event);

	int i = 0;
	switch (i)
	{
	case 0:
		break;
	default:
		break;
	}

	switch (event.type)
	{
	case SDL_QUIT:
		ECS_serialiseAllComponents(&game->components, "./saves/");
		//free(game->components.positionComponents);
		//free(game->components.textureComponents);
		game->isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			Position_moveBy(&game->components.positionComponents[1], (Vec2) { -3, 0 });
			break;
		case SDLK_UP:
			Position_moveBy(&game->components.positionComponents[1], (Vec2) { 0, -3 });
			break;
		case SDLK_RIGHT:
			Position_moveBy(&game->components.positionComponents[1], (Vec2) { 3, 0 });
			break;
		case SDLK_DOWN:
			Position_moveBy(&game->components.positionComponents[1], (Vec2) { 0, 3 });
			break;
		case SDLK_s:
			ECS_serialiseAllComponents(&game->components, "./saves/");
			break;
		case SDLK_l:
			ECS_deserialiseAllComponents(&game->components, "./saves/");
			break;
		case SDLK_p:
		{
			ps(game);
			break;
		}
		default:
			break;
		}
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
