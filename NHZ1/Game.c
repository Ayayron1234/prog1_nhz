#include "Game.h"


void Game_init(Game *game, char windowName[255], Vec2Int windowDimensions, bool isFullScreen, RGBAColor backgroundColor)
{
	game->appearance = (GameAppearance){ .backgroundColor = backgroundColor, .isFullScrean = isFullScreen, .windowDimensions = windowDimensions};
	strcpy_s(game->appearance.windowName, 255, windowName);
	game->camera = (Vec2){ 0, 0 };

	// load components to heap
	game->components = ECS_init(128, true, "./saves/", &game->tilemap);

	// set gamestate
	game->state = EDIT_MODE;

	ECS_getPositionComponent(&game->components, 4)->value = (Vec2){ 768, 336 };
	ECS_getPhysicsBodyComponent(&game->components, 4)->velocity = (Vec2){ 100, 0 };
	//ECS_getPhysicsBodyComponent(&game->components, 4)->mass = 90;
	//ECS_getPhysicsBodyComponent(&game->components, 3)->gravitationalAcceleration = (Vec2){ 0, 0 };
	//ECS_getPhysicsBodyComponent(&game->components, 4)->gravitationalAcceleration = (Vec2){ 0, 16 };
	//ECS_getPhysicsBodyComponent(&game->components, 3)->acceleration = (Vec2){ 0, 0 };

	// initialise sdl
	Game_sdlInit(game);
	// load tilemap
	Tilemap_init(&game->tilemap, (Vec2Int) { 48, 48 }, game->renderer, "./saves/tilemap.png");
	// init game time
	game->time.deltaT = 1;
	game->time.lastUpdateTime = SDL_GetTicks();
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

	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] != 0) {
		ECS_getAnimationComponent(&game->components, 1)->tilePosition.x = 2;
		ECS_getAnimationComponent(&game->components, 1)->frameCount = 3;
		ECS_getPhysicsBodyComponent(&game->components, 1)->velocity.x = -300;
	}
	else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] != 0) {
		ECS_getAnimationComponent(&game->components, 1)->tilePosition.x = 7;
		ECS_getAnimationComponent(&game->components, 1)->frameCount = 3;
		ECS_getPhysicsBodyComponent(&game->components, 1)->velocity.x = 300;
	}
	else {
		ECS_getAnimationComponent(&game->components, 1)->tilePosition.x = 5;
		ECS_getAnimationComponent(&game->components, 1)->frameCount = 1;
		ECS_getPhysicsBodyComponent(&game->components, 1)->velocity.x = 0;
	}
	if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] != 0) {
		Vec2 pos = ECS_getPositionComponent(&game->components, 1)->value;
		Vec2 size = ECS_getCollisionBoxComponent(&game->components, 1)->size;
		Vec2 a = { pos.x, pos.y + size.y + 1 };
		Vec2 b = { pos.x + size.x, pos.y + size.y + 1 };

		bool isOnFloor = false;
		for (int i = 0; i < game->components.total_colliderComponents; i++) {
			CollisionBox* box = ECS_getCollisionBoxComponent(&game->components, game->components.colliderComponents[i].ENTITY_ID);
			if (NULL == box) continue;
			if (CollisionBox_isPointInside(&game->components, box, a) || CollisionBox_isPointInside(&game->components, box, b)) isOnFloor = true;
		}
		if (isOnFloor)
			ECS_getPhysicsBodyComponent(&game->components, 1)->velocity.y = -800;
	}

	// update components
	if (game->state == EDIT_MODE) {
		for (int i = 0; i < game->components.total_spriteComponents; i++)
			Sprite_edit_update(&game->components, &game->components.spriteComponents[i], game->renderer);
		for (int i = 0; i < game->components.total_tileComponents; i++)
			Tile_edit_update(&game->components, &game->components.tileComponents[i], game->renderer);
	}
	if (game->state == GAME_MODE) {
		for (int i = 0; i < game->components.total_physicsBodyComponents; i++)
			PhysicsBody_update(&game->components, &game->components.physicsBodyComponents[i], game->time.deltaT / 1000.0);
	}
	for (int i = 0; i < game->components.total_collisionBoxComponents; i++)
		CollisionBox_update(game->state, &game->components, &game->components.collisionBoxComponents[i]);
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
		Text_serialise(game->components.textComponents, 128, "./saves/text.data");
		CollisionBox_serialise(game->components.collisionBoxComponents, 128, "./saves/collisionBox.data");
		Collider_serialise(game->components.colliderComponents, 128, "./saves/collider.data");
		Collider_serialise(game->components.physicsBodyComponents, 128, "./saves/physicsBody.data");
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
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->size.x = (tile->size.x > 1) ? tile->size.x - 1 : 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->tilePosition.x = (tile->tilePosition.x > 0) ? tile->tilePosition.x - 1 : 0;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)), (Vec2) { -12, 0 });
				}
			}
			break;
		case SDLK_UP:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->size.y = (tile->size.y > 1) ? tile->size.y - 1 : 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->tilePosition.y = (tile->tilePosition.y > 0) ? tile->tilePosition.y - 1 : 0;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)),(Vec2) { 0, -12 });
				}
			}
			break;
		case SDLK_RIGHT:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->size.x = tile->size.x + 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->tilePosition.x = tile->tilePosition.x + 1;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)), (Vec2) { 12, 0 });
				}
			}
			break;
		case SDLK_DOWN:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->size.y = tile->size.y + 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
					if (NULL != tile) tile->tilePosition.y = tile->tilePosition.y + 1;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(&game->components)), (Vec2) { 0, 12 });
				}
			}
			break;
		case SDLK_c:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Editor_copy(&game->components, Editor_getSelected(&game->components));
				}
			}
			break;
		case SDLK_v:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					int newEntity = ECS_createEntity(&game->components, 128);
					Editor_paste(&game->components, newEntity);
				}
			}
			break;
		case SDLK_DELETE:
			if (game->state == EDIT_MODE) {
				ECS_deleteEntity(&game->components, Editor_getSelected(&game->components));
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
	SDL_SetRenderDrawColor(game->renderer, game->appearance.backgroundColor.r, game->appearance.backgroundColor.g, game->appearance.backgroundColor.b, game->appearance.backgroundColor.a);
	SDL_RenderClear(game->renderer);

	for (int i = 0; i < game->components.total_tileComponents; i++) {
		Tile_render(&game->components, &game->components.tileComponents[i], game->renderer);
	}
	for (int i = 0; i < game->components.total_spriteComponents; i++) {
		Sprite_render(&game->components, &game->components.spriteComponents[i], game->renderer);
	}
	for (int i = 0; i < game->components.total_textComponents; i++) {
		Text_render(&game->components, &game->components.textComponents[i], game->renderer);
	}
	for (int i = 0; i < game->components.total_editorComponents; i++) {
		Editor_render(game->state, &game->components, &game->components.editorComponents[i], game->renderer);
	}

	if (game->state == EDIT_MODE) {
		Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(&game->components));
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

	//int x, y;
	//Vec2 contactPoint;
	//Vec2 contactNormal;
	//double t;
	//SDL_PumpEvents();
	//Uint32 buttons = SDL_GetMouseState(&x, &y);
	//Position* physcsPos = ECS_getPositionComponent(&game->components, 3);
	//Ray2 ray = (Ray2){ (Vec2) { x, y }, (Vec2) { physcsPos->value.x, physcsPos->value.y } };
	//if (buttons & SDL_BUTTON_LMASK) {
	//	ECS_getPhysicsBodyComponent(&game->components, 3)->velocity = (Vec2){ x - physcsPos->value.x, y - physcsPos->value.y };
	//	SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
	//}
	//else
	//	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
	//SDL_RenderDrawLine(game->renderer, ray.origin.x, ray.origin.y, ray.destination.x, ray.destination.y);


	SDL_RenderPresent(game->renderer);
}
