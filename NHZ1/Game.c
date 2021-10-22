#include "Game.h"


Position* fillTestArray() {
	Position* arr;
	arr = (Position*)calloc(100, sizeof(struct Position));

	return arr;
}

void Game_init(Game *game, char windowName[255], Vec2Int windowDimensions, bool isFullScreen, RGBAColor backgroundColor)
{
	game->appearance = (GameAppearance){ .backgroundColor = backgroundColor, .isFullScrean = isFullScreen, .windowDimensions = windowDimensions };
	strcpy_s(game->appearance.windowName, 255, windowName);

	// load components to heap
	game->components = ECS_init(128, true, "./saves/", &game->tilemap);
	// find componentlist with most components and set entities to it's length
	game->ENTITIES = 0;
	if (game->components.total_positionComponents > game->ENTITIES) game->ENTITIES = game->components.total_positionComponents;
	if (game->components.total_spriteComponents > game->ENTITIES) game->ENTITIES = game->components.total_spriteComponents;
	if (game->components.total_editorComponents > game->ENTITIES) game->ENTITIES = game->components.total_editorComponents;
	if (game->components.total_animationComponents > game->ENTITIES) game->ENTITIES = game->components.total_animationComponents;
	if (game->components.total_tileComponents > game->ENTITIES) game->ENTITIES = game->components.total_tileComponents;

	// initialise sdl
	Game_sdlInit(game);
	// load tilemap
	Tilemap_init(&game->tilemap, (Vec2Int) { 48, 48 }, game->renderer, "./saves/tilemap.png");
}

void Game_sdlInit(Game *game) {
	int flags = SDL_WINDOW_RESIZABLE;
	flags = (game->appearance.isFullScrean) ? flags | SDL_WINDOW_FULLSCREEN : flags;

	// Initialise SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		SDL_Log("SDL cound not be initialised: %s\n", SDL_GetError());
		return;
	}

	// Create window
	game->window = SDL_CreateWindow(game->appearance.windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
		game->appearance.windowDimensions.x, game->appearance.windowDimensions.y, flags);
	if (game->window == NULL)
	{
		SDL_Log("SDL couldn't open %s window: %s\n", game->appearance.windowName, SDL_GetError());
		return;
	}

	// Create rendering context
	game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_SOFTWARE);
	if (game->renderer == NULL)
	{
		SDL_Log("SDL couldn't create rendering context: %s", SDL_GetError());
		return;
	}
	SDL_SetRenderDrawBlendMode(game->renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderClear(game->renderer);
}

void Game_update(Game* game)
{
	// deselect everything if nothing was clicked
	int x, y;
	Uint32 buttons;
	SDL_PumpEvents();
	buttons = SDL_GetMouseState(&x, &y);
	if ((buttons & SDL_BUTTON_LMASK) != 0) {
		bool clickedNothing = true;
		for (int i = 0; i < game->components.total_spriteComponents; i++) {
			if (Sprite_isPointInside(&game->components, &game->components.spriteComponents[i], (Vec2) { x, y }))
				clickedNothing = false;
		}
		for (int i = 0; i < game->components.total_tileComponents; i++) {
			if (Tile_isPointInside(&game->components, &game->components.tileComponents[i], (Vec2) { x, y }))
				clickedNothing = false;
		}
		if (clickedNothing)
			Editor_deselectAll(&game->components);
	}

	// update components
	for (int i = 0; i < game->components.total_spriteComponents; i++)
		Sprite_edit_update(&game->components, &game->components.spriteComponents[i], game->renderer);
	for (int i = 0; i < game->components.total_tileComponents; i++)
		Tile_edit_update(&game->components, &game->components.tileComponents[i], game->renderer);

	for (int i = 0; i < game->components.total_animationComponents; i++)
		Animation_update(&game->components, &game->components.animationComponents[i]);
}

void Game_handleSDLEvents(Game* game)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		// serialise_components
		Position_serialise(game->components.positionComponents, 128, "./saves/position.data");
		Sprite_serialise(game->components.spriteComponents, 128, "./saves/sprite.data");
		Editor_serialise(game->components.editorComponents, 128, "./saves/editor.data");
		Animation_serialise(game->components.animationComponents, 128, "./saves/animation.data");
		Tile_serialise(game->components.tileComponents, 128, "./saves/tile.data");
		// stop game
		game->isRunning = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)),
				(Vec2) { -3, 0 });
			break;
		case SDLK_UP:
			Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)),
				(Vec2) { 0, -3 });
			break;
		case SDLK_RIGHT:
			Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)),
				(Vec2) { 3, 0 });
			break;
		case SDLK_DOWN:
			Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)),
				(Vec2) { 0, 3 });
			break;
		case SDLK_p: {
			//Position test[128];
			//for (int i = 0; i < game->components.total_positionComponents; i++) {
			//	test[i] = game->components.positionComponents[i];
			//	printf("Entity#%d: Vec2(%g,%g)\n", game->components.positionComponents[i].ENTITY_ID, game->components.positionComponents[i].value.x, game->components.positionComponents[i].value.y);
			//}
			break;
		}
		default:
			break;
		}
	default:
		break;
	}
}

void Game_renderElements(Game* game)
{
	SDL_SetRenderDrawColor(game->renderer, game->appearance.backgroundColor.r, game->appearance.backgroundColor.g, game->appearance.backgroundColor.b, game->appearance.backgroundColor.a );
	SDL_RenderClear(game->renderer);

	for (int i = 0; i < game->components.total_tileComponents; i++) {
		Tile_render(&game->components, &game->components.tileComponents[i], game->renderer);
	}
	for (int i = 0; i < game->components.total_spriteComponents; i++) {
		Sprite_render(&game->components, &game->components.spriteComponents[i], game->renderer);
	}
	for (int i = 0; i < game->components.total_editorComponents; i++) {
		Editor_render(&game->components, &game->components.editorComponents[i], game->renderer);
	}

	SDL_RenderPresent(game->renderer);
}
