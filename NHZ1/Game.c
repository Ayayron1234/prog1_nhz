#include "Game.h"


void Game_init(Game *game, char windowName[255], Vec2Int windowDimensions, bool isFullScreen, RGBAColor backgroundColor)
{
	game->appearance = (GameAppearance){ .backgroundColor = backgroundColor, .isFullScrean = isFullScreen, .windowDimensions = windowDimensions};
	strcpy_s(game->appearance.windowName, 255, windowName);

	// initialise sdl
	Game_sdlInit(game);
	game->resources.mainFont = TTF_OpenFont("uni.ttf", 16);

	// load tilemap
	Tilemap_init(&game->tilemap, (Vec2Int) { 48, 48 }, game->renderer, "./saves/tilemap.png");
	Tilemap_init(&game->resources.tilemap, (Vec2Int) { 48, 48 }, game->renderer, "./saves/tilemap.png");

	// set gamestate
	game->state = EDIT_MODE;

	ECS_load(&game->layouts, &game->numberOfLayouts, "saves/aaron.data", &game->resources);
	//ECS_load(&game->layouts, &game->componentLists, &game->numberOfLayouts, "saves/aaron.data", &game->resources);
	game->currentLayout = &game->layouts[0];

	((Text*)ECS_getComponent(TEXT, *game->currentLayout, 22))->fontSize = 16;

	((Position*)ECS_getComponent(POSITION, *game->currentLayout, 4))->value = (Vec2){ 768, 336 };
	((PhysicsBody*)ECS_getComponent(PHYSICS_BODY, *game->currentLayout, 4))->velocity = (Vec2){ 100, 0 };

	ECS_getLayout(game->layouts, game->numberOfLayouts, "map1")->layers[3].parallax = (Vec2){ 0.7,1 };
	ECS_getLayout(game->layouts, game->numberOfLayouts, "map1")->layers[3].zIndex = 1;

	// init game time
	game->time.deltaT = 1;
	game->time.lastUpdateTime = SDL_GetTicks();
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

	// Init TTF
	if (TTF_Init() == -1) {
		printf("TTF_Init: %s\n", TTF_GetError());
		exit(2);
	}
}

void Game_update(Game* game)
{
	// update game time
	long int now = SDL_GetTicks();
	game->time.deltaT = now - game->time.lastUpdateTime;
	game->time.lastUpdateTime = now;

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

					bool isOnFloor = false;
					for (int i = 0; i < ECS_getNumberOfComponents(COLLIDER, *game->currentLayout); i++) {
						CollisionBox* box = ECS_getComponent(COLLISION_BOX, *game->currentLayout, ((CollisionBox*)ECS_getNthComponent(COLLIDER, game->currentLayout, i))->ENTITY_ID);
						if (NULL == box) continue;
						if (CollisionBox_isPointInside(game->currentLayout, box, a) || CollisionBox_isPointInside(game->currentLayout, box, b)) isOnFloor = true;
					}
					if (isOnFloor)
						((PhysicsBody*)mario[PHYSICS_BODY])->velocity.y = -800;
				}
				int windowWidth;
				SDL_GetWindowSize(game->window, &windowWidth, NULL);

				Position* marioPosition = mario[POSITION];
				if ((game->currentLayout->camera.x + windowWidth / 2) < marioPosition->value.x - windowWidth / 8) 
					game->currentLayout->camera = (Vec2){ marioPosition->value.x - windowWidth / 2 - windowWidth / 8, 0};			
				if ((game->currentLayout->camera.x + windowWidth / 2) > marioPosition->value.x + windowWidth / 8 && game->currentLayout->camera.x > 0)
					game->currentLayout->camera = (Vec2){ marioPosition->value.x - windowWidth / 2 + windowWidth / 8, 0 };
				if (game->currentLayout->camera.x < 0) game->currentLayout->camera = (Vec2){ 0, 0 };
			
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

	// update components
	if (game->state == EDIT_MODE) {
		//for (int i = 0; i < game->components.total_spriteComponents; i++)
		//	Sprite_edit_update(&game->components, &game->components.spriteComponents[i], game->renderer);
		//for (int i = 0; i < game->components.total_tileComponents; i++)
		//	Tile_edit_update(&game->components, &game->components.tileComponents[i], game->renderer);
	}
	if (game->state == GAME_MODE) {
		for (int i = 0; i < ECS_getNumberOfComponents(PHYSICS_BODY, *game->currentLayout); i++)
			PhysicsBody_update(game->currentLayout, ECS_getNthComponent(PHYSICS_BODY, game->currentLayout, i), game->time.deltaT / 1000.0);
	}
	for (int i = 0; i < ECS_getNumberOfComponents(COLLISION_BOX, *game->currentLayout); i++)
		CollisionBox_update(game->state, game->currentLayout, ECS_getNthComponent(COLLISION_BOX, game->currentLayout, i));
	for (int i = 0; i < ECS_getNumberOfComponents(ANIMATION, *game->currentLayout); i++)
		Animation_update(*game->currentLayout, ECS_getNthComponent(ANIMATION, game->currentLayout, i));
	for (int i = 0; i < ECS_getNumberOfComponents(EDITOR, *game->currentLayout); i++)
		Editor_update(game->state, game->currentLayout, ECS_getNthComponent(EDITOR, game->currentLayout, i));
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
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_F5:
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
			int currentIndex = ECS_getLayoutIndex(game->layouts, game->numberOfLayouts, game->currentLayout->LAYOUT_NAME);
			if (currentIndex < game->numberOfLayouts - 1) game->currentLayout = &game->layouts[currentIndex + 1];
			else game->currentLayout = &game->layouts[0];
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
					Editor_paste(game->layouts, game->numberOfLayouts, &game->currentLayout->LAYOUT_NAME);
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
				
				Position pos[100];
				for (int i = 0; i < 60; i++)
					pos[i] = ((Position*)game->currentLayout->componentListsPointers[POSITION])[i];
				printf("");
				//free(entity);
			}
			break;
		default:
			break;
		}
	default:
		break;
	}
}

void Game_renderElements(Game* game)
{
	SDL_SetRenderDrawColor(game->renderer, game->appearance.backgroundColor.r, game->appearance.backgroundColor.g, game->appearance.backgroundColor.b, game->appearance.backgroundColor.a);
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
		Editor_render(game->state, game->currentLayout, &((Editor*)ECS_getComponentList(EDITOR, *game->currentLayout))[i], game->renderer);
	}

	// display tilemap to help with texturing
	if (game->state == EDIT_MODE) {
		Tile* tile = ECS_getComponent(TILE, *game->currentLayout, Editor_getSelected(game->currentLayout));
		if (NULL != tile && SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
			SDL_Rect rect = { 40, 40, game->tilemap.tileSize.x / 0.2, game->tilemap.tileSize.y / 0.2 };
			SDL_RenderCopy(game->renderer, game->tilemap.texture, NULL, &rect);

			SDL_Rect selRect = {
				40 + tile->tilePosition.x * game->tilemap.tileSize.x / 2, 40 + tile->tilePosition.y * game->tilemap.tileSize.y / 2,
				game->tilemap.tileSize.x / 2, game->tilemap.tileSize.y / 2
			};
			SDL_SetRenderDrawColor(game->renderer, 255, 180, 130, 255);
			SDL_RenderDrawRect(game->renderer, &selRect);
			SDL_SetRenderDrawColor(game->renderer, 255, 180, 130, 100);
			SDL_RenderFillRect(game->renderer, &selRect);
		}
	}

	SDL_RenderPresent(game->renderer);
}
