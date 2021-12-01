#include "Game.h"
#include "windows.h"

void Game_init(Game *game, char windowName[255], Vec2Int windowDimensions, bool isFullScreen, RGBAColor backgroundColor)
{
	game->appearance = (GameAppearance){ .backgroundColor = backgroundColor, .isFullScrean = isFullScreen, .windowDimensions = windowDimensions};
	strcpy(game->appearance.windowName, windowName);

	// initialise sdl
	Game_sdlInit(game);
	game->resources.mainFont = TTF_OpenFont("uni.ttf", 16);

	// load tilemap
	Tilemap_init(&game->resources.tilemap, (Vec2Int) { 48, 48 }, game->renderer, "./saves/tilemap.png");

	// set gamestate
	game->state = GAME_MODE;

	ECS_load(&game->layouts, &game->numberOfLayouts, "original", &game->resources);
	game->currentLayout = &game->layouts[1];
	strcpy(&game->player, "original");

	// init game time
	game->time.deltaT = 1;
	game->time.lastUpdateTime = SDL_GetTicks();
	game->time.timeSinceStart = 0;

	Game_setup(game);
}

void Game_sdlInit(Game *game) {
	int flags = SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC;
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
	ECS__debug_renderer = game->renderer;

	// Init TTF
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
}

void Game_setup(Game* game) {
	
}

void Game_handleSDLEvents(Game* game)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		// stop game
		game->isRunning = false;
		break;
	case SDL_KEYDOWN: {
		// input to textbox with focus
		Interactable* interactable = Interactabe_getFocused(game->currentLayout);
		if (NULL != interactable) {
			InteractionData_keydown* data = (InteractionData_keydown*)malloc(sizeof(InteractionData_keydown));
			if (data == NULL) break;
			if ((char)event.key.keysym.sym == 13) {
				data->specialKey = 13;
				data->layoutsPtr = &game->layouts;
				data->numberOfLayoutsPtr = &game->numberOfLayouts;
				data->gameResourcesPtr = &game->resources;
				data->playerName = game->player;
				data->currentLayoutPtr = &game->currentLayout;
			}
			else {
				data->key = (char)event.key.keysym.sym;
				data->mod = event.key.keysym.mod;
			}
			Interactable_interact(interactable, KEYDOWN, data);
		}

		switch (event.key.keysym.sym)
		{
		case SDLK_F5:
			if (game->state == GAME_MODE) {
				ECS_switchToPlayer(game->player, "original", &game->layouts, &game->numberOfLayouts, &game->currentLayout, &game->resources);
			}
			if (game->state == EDIT_MODE) {
				game->currentLayout = ECS_getLayout(game->layouts, game->numberOfLayouts, "menu");
			}
			game->state = (game->state == EDIT_MODE) ? GAME_MODE : EDIT_MODE;
			break;
		case SDLK_LEFT:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.x = (tile->size.x > 1) ? tile->size.x - 1 : 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.x = (tile->tilePosition.x > 0) ? tile->tilePosition.x - 1 : 0;
				}
				else {
					Position_moveBy(ECS_getComponent(POSITION, *game->currentLayout, Editor_getSelected(game->currentLayout)), (Vec2) { -12, 0 });
				}
			}
			break;
		case SDLK_UP:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.y = (tile->size.y > 1) ? tile->size.y - 1 : 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.y = (tile->tilePosition.y > 0) ? tile->tilePosition.y - 1 : 0;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] != 0) {
					EntityRenderer* renderer = ECS_getComponent(ENTITY_RENDERER, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != renderer && renderer->layerIndex < 16) renderer->layerIndex++;
				}
				else {
					Position_moveBy(ECS_getComponent(POSITION, *game->currentLayout, Editor_getSelected(game->currentLayout)), (Vec2) { 0, -12 });
				}
			}
			break;
		case SDLK_RIGHT:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.x = tile->size.x + 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.x = tile->tilePosition.x + 1;
				}
				else {
					Position_moveBy(ECS_getComponent(POSITION, *game->currentLayout, Editor_getSelected(game->currentLayout)), (Vec2) { 12, 0 });
				}
			}
			break;
		case SDLK_DOWN:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.y = tile->size.y + 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.y = tile->tilePosition.y + 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT] != 0) {
					EntityRenderer* renderer = ECS_getComponent(ENTITY_RENDERER, *game->currentLayout, Editor_getSelected(game->currentLayout));
					if (NULL != renderer && renderer->layerIndex > 0) renderer->layerIndex--;
				}
				else {
					Position_moveBy(ECS_getComponent(POSITION, *game->currentLayout, Editor_getSelected(game->currentLayout)), (Vec2) { 0, 12 });
				}
			}
			break;
		case SDLK_SPACE: {
			if (game->state == EDIT_MODE) {
				int currentIndex = ECS_getLayoutIndex(game->layouts, game->numberOfLayouts, game->currentLayout->LAYOUT_NAME);
				if (currentIndex < game->numberOfLayouts - 1) game->currentLayout = &game->layouts[currentIndex + 1];
				else game->currentLayout = &game->layouts[0];
			}
		}
		break;
		case SDLK_c:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Editor_copy(game->currentLayout, Editor_getSelected(game->currentLayout));
				}
			}
			break;
		case SDLK_v:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Editor_paste(game->layouts, game->numberOfLayouts, &game->currentLayout->LAYOUT_NAME, NULL);
				}
			}
			break;
		case SDLK_DELETE:
			if (game->state == EDIT_MODE) {
				int selectedID = Editor_getSelected(game->currentLayout);
				void** entity = ECS_getEntity(*game->currentLayout, selectedID);
				if (NULL == entity) break;

				for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++)
					if (entity[i] != NULL) ECS_deleteComponent(i, game->layouts, game->numberOfLayouts, game->currentLayout->LAYOUT_NAME, selectedID);
				
				free(entity);
			}
			break;
		default: {
			break;
		}
	}
		}
	default:
		break;
	}
}

void Game_update(Game* game)
{
	if (game->state == VICTORY) {
		double score = 100 / (1 + (game->time.timeSinceStart / 10000.0));
		
		FILE* f = fopen("./saves/scores.txt", "a");
		if (f != NULL) {
			fprintf(f, "%s %g\n", game->player, score);
			fclose(f);
		}

		char path[256];
		sprintf(path, "./saves/%s.data", game->player);
		remove(path);

		game->isRunning = false;
	}

	// update game time
	long int now = SDL_GetTicks();
	game->time.deltaT = now - game->time.lastUpdateTime;
	game->time.lastUpdateTime = now;
	game->time.timeSinceStart += game->time.deltaT;

	if (game->state == EDITOR) {
		int x, y;
		Uint32 buttons;
		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&x, &y);

		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			Editor_selectEntityAtClick(game->currentLayout, (Vec2) { x, y });
		}
	}

	if (game->state == GAME_MODE) {
		int x, y;
		Uint32 buttons;
		SDL_PumpEvents();
		buttons = SDL_GetMouseState(&x, &y);

		if ((buttons & SDL_BUTTON_LMASK) != 0) {
			Interactable_focusEntityAtClick(game->currentLayout, (Vec2) { x, y });
		}

		if (ECS_layoutHasName(game->currentLayout, "menu")) {
			game->currentLayout->camera = (Vec2){ 0, 0 };
		}
		if (ECS_layoutHasName(game->currentLayout, "map1") || ECS_layoutHasName(game->currentLayout, "map2")) {
			void** mario = ECS_getEntity(*game->currentLayout, 1);
			if (NULL != mario) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] != 0) {
					((Animation*)mario[ANIMATION])->tilePosition.x = 2;
					((Animation*)mario[ANIMATION])->frameCount = 3;
					((PhysicsBody*)mario[PHYSICS_BODY])->velocity.x = -300;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] != 0) {
					((Animation*)mario[ANIMATION])->tilePosition.x = 7;
					((Animation*)mario[ANIMATION])->frameCount = 3;
					((PhysicsBody*)mario[PHYSICS_BODY])->velocity.x = 300;
				}
				else {
					((Animation*)mario[ANIMATION])->tilePosition.x = 5;
					((Animation*)mario[ANIMATION])->frameCount = 1;
					((PhysicsBody*)mario[PHYSICS_BODY])->velocity.x = 0;
				}
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] != 0) {
					Vec2 pos = ((Position*)mario[POSITION])->value;
					Vec2 size = ((CollisionBox*)mario[COLLISION_BOX])->size;
					Vec2 a = { pos.x, pos.y + size.y + 1 };
					Vec2 b = { pos.x + size.x, pos.y + size.y + 1 };
				}
				int windowWidth;
				SDL_GetWindowSize(game->window, &windowWidth, NULL);

				Position* marioPosition = mario[POSITION];
				if ((game->currentLayout->camera.x + windowWidth / 2) < marioPosition->value.x - windowWidth / 8) 
					game->currentLayout->camera = (Vec2){ marioPosition->value.x - windowWidth / 2 - windowWidth / 8, 0};			
				if ((game->currentLayout->camera.x + windowWidth / 2) > marioPosition->value.x + windowWidth / 8 && game->currentLayout->camera.x > 0)
					game->currentLayout->camera = (Vec2){ marioPosition->value.x - windowWidth / 2 + windowWidth / 8, 0 };
				if (game->currentLayout->camera.x < 0) game->currentLayout->camera = (Vec2){ 0, 0 };
				
				if (marioPosition->value.y > 1000)
					marioPosition->value = (Vec2){ 281, 456 };

				ECS_freeEntity(mario);
			}
		}
	}

	if (game->state == EDIT_MODE) {
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] != 0 && Editor_getSelected(game->currentLayout) == 0)
			game->currentLayout->camera.x -= game->time.deltaT / 1.5;
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] != 0 && Editor_getSelected(game->currentLayout) == 0)
			game->currentLayout->camera.x += game->time.deltaT / 1.5;
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] != 0 && Editor_getSelected(game->currentLayout) == 0)
			game->currentLayout->camera.y -= game->time.deltaT / 1.5;
		if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN] != 0 && Editor_getSelected(game->currentLayout) == 0)
			game->currentLayout->camera.y += game->time.deltaT / 1.5;
	}

	if (game->state == GAME_MODE) {
		for (int i = 0; i < ECS_getNumberOfComponents(PHYSICS_BODY, *game->currentLayout); i++)
			PhysicsBody_update(game->currentLayout, ECS_getNthComponent(PHYSICS_BODY, game->currentLayout, i), game->time.deltaT / 1000.0, &game->state);
		for (int i = 0; i < ECS_getNumberOfComponents(MOVEMENT_CONTROLLER, *game->currentLayout); i++)
			MovementController_update(game->currentLayout, ECS_getNthComponent(MOVEMENT_CONTROLLER, game->currentLayout, i), game->time.deltaT);
		for (int i = 0; i < ECS_getNumberOfComponents(INTERACTABLE, *game->currentLayout); i++)
			Interactable_update(game->layouts, game->numberOfLayouts, game->currentLayout->LAYOUT_NAME, ECS_getNthComponent(INTERACTABLE, game->currentLayout, i), game->state);

		// BUG: I shouldn't need to do this. Idk why it doesn't work without it
		for (int i = 0; i < ECS_getNumberOfComponents(INTERACTABLE, *game->currentLayout); i++)
			((Interactable*)ECS_getNthComponent(INTERACTABLE, game->currentLayout, i))->interactionHead = NULL;
	}

	for (int i = 0; i < ECS_getNumberOfComponents(COLLISION_BOX, *game->currentLayout); i++)
		CollisionBox_update(game->state, game->currentLayout, ECS_getNthComponent(COLLISION_BOX, game->currentLayout, i));
	for (int i = 0; i < ECS_getNumberOfComponents(ANIMATION, *game->currentLayout); i++)
		Animation_update(*game->currentLayout, ECS_getNthComponent(ANIMATION, game->currentLayout, i));
}

void Game_renderElements(Game* game)
{
	SDL_SetRenderDrawColor(game->renderer, game->appearance.backgroundColor.r, game->appearance.backgroundColor.g, game->appearance.backgroundColor.b, game->appearance.backgroundColor.a);
	if (ECS_layoutHasName(game->currentLayout, "menu"))
		SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	for (int i = 0; i < ECS_getNumberOfComponents(TILE, *game->currentLayout); i++) {
		Tile_render(game->currentLayout, &((Tile*)ECS_getComponentList(TILE, *game->currentLayout))[i], game->renderer);
	}
	for (int i = 0; i < ECS_getNumberOfComponents(SPRITE, *game->currentLayout); i++) {
		Sprite_render(game->currentLayout, &((Sprite*)ECS_getComponentList(SPRITE, *game->currentLayout))[i], game->renderer);
	}
	for (int i = 0; i < ECS_getNumberOfComponents(TEXT, *game->currentLayout); i++) {
		Text_render(game->currentLayout, &((Text*)ECS_getComponentList(TEXT, *game->currentLayout))[i], game->renderer, game->resources.mainFont);
	}
	for (int i = 0; i < ECS_getNumberOfComponents(EDITOR, *game->currentLayout); i++) {
		Editor_render(game->state, game->currentLayout, &((Editor*)ECS_getComponentList(EDITOR, *game->currentLayout))[i], game->renderer, game->resources.mainFont);
	}

	// display tilemap to help with texturing
	if (game->state == EDIT_MODE) {
		Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
		if (NULL != tile && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
			SDL_Rect rect = { 40, 40, game->resources.tilemap.tileSize.x / 0.2, game->resources.tilemap.tileSize.y / 0.2 };
			SDL_RenderCopy(game->renderer, game->resources.tilemap.texture, NULL, &rect);

			SDL_Rect selRect = {
				40 + tile->tilePosition.x * game->resources.tilemap.tileSize.x / 2, 40 + tile->tilePosition.y * game->resources.tilemap.tileSize.y / 2,
				game->resources.tilemap.tileSize.x / 2, game->resources.tilemap.tileSize.y / 2
			};
			SDL_SetRenderDrawColor(game->renderer, 255, 180, 130, 255);
			SDL_RenderDrawRect(game->renderer, &selRect);
			SDL_SetRenderDrawColor(game->renderer, 255, 180, 130, 100);
			SDL_RenderFillRect(game->renderer, &selRect);
		}
	}

	SDL_RenderPresent(game->renderer);
}
