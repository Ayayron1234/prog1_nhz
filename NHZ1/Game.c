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

	// load components to heap
	//game->components = ECS_init(128, true, "./saves/", &game->tilemap);

	// set gamestate
	game->state = EDIT_MODE;

	//ECS_getPositionComponent(&game->components, 4)->value = (Vec2){ 768, 336 };
	//ECS_getPhysicsBodyComponent(&game->components, 4)->velocity = (Vec2){ 100, 0 };

	//game->componentLists = malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(void*));
	//if (NULL == game->componentLists) exit(1);
	//game->componentLists[POSITION] = game->components.positionComponents;
	//game->componentLists[EDITOR] = game->components.editorComponents;
	//game->componentLists[SPRITE] = game->components.spriteComponents;
	//game->componentLists[TILE] = game->components.tileComponents;
	//game->componentLists[COLLIDER] = game->components.colliderComponents;
	//game->componentLists[COLLISION_BOX] = game->components.collisionBoxComponents;
	//game->componentLists[PHYSICS_BODY] = game->components.physicsBodyComponents;
	//game->componentLists[TEXT] = game->components.textComponents;
	//game->componentLists[ANIMATION] = game->components.animationComponents;

	//game->layouts = malloc(sizeof(Layout));
	//game->layouts[0].componentMaps = malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(LayoutMap));
	//game->layouts[0].componentMaps[POSITION].end = game->components.total_positionComponents;
	//game->layouts[0].componentMaps[EDITOR].end = game->components.total_editorComponents;
	//game->layouts[0].componentMaps[SPRITE].end = game->components.total_spriteComponents;
	//game->layouts[0].componentMaps[TILE].end = game->components.total_tileComponents;
	//game->layouts[0].componentMaps[COLLIDER].end = game->components.total_colliderComponents;
	//game->layouts[0].componentMaps[COLLISION_BOX].end = game->components.total_collisionBoxComponents;
	//game->layouts[0].componentMaps[PHYSICS_BODY].end = game->components.total_physicsBodyComponents;
	//game->layouts[0].componentMaps[TEXT].end = game->components.total_textComponents;
	//game->layouts[0].componentMaps[ANIMATION].end = game->components.total_animationComponents;

	//game->layouts[0].componentListsPointers = malloc(NUMBER_OF_COMPONENT_TYPES * sizeof(void*));
	//game->layouts[0].componentListsPointers[POSITION] = game->components.positionComponents;
	//game->layouts[0].componentListsPointers[EDITOR] = game->components.editorComponents;
	//game->layouts[0].componentListsPointers[SPRITE] = game->components.spriteComponents;
	//game->layouts[0].componentListsPointers[TILE] = game->components.tileComponents;
	//game->layouts[0].componentListsPointers[COLLIDER] = game->components.colliderComponents;
	//game->layouts[0].componentListsPointers[COLLISION_BOX] = game->components.collisionBoxComponents;
	//game->layouts[0].componentListsPointers[PHYSICS_BODY] = game->components.physicsBodyComponents;
	//game->layouts[0].componentListsPointers[TEXT] = game->components.textComponents;
	//game->layouts[0].componentListsPointers[ANIMATION] = game->components.animationComponents;

	//for (int i = 0; i < NUMBER_OF_COMPONENT_TYPES; i++)
	//	game->layouts[0].componentMaps[i].start = 0;


	ECS_load(&game->layouts, &game->componentLists, "saves/aaron.data", &game->resources);
	game->numberOfLayouts = 1;
	game->currentLayout = &game->layouts[0];
	//strcpy(game->currentLayout->LAYOUT_NAME, "map1");

	((Text*)ECS_getComponent(TEXT, *game->currentLayout, 22))->fontSize = 16;

	Sprite test[61];
	for (int i = 0; i < 3; i++)
		test[i] = ((Sprite*)game->componentLists[SPRITE])[i];

	//Position* pos = ((Position*)ECS_getComponent(POSITION, *game->currentLayout, 1));

	//printf("%g, %g\n", pos->value.x, pos->value.y);

	//printf("%d, %d", ECS_getNumberOfComponents(TILE, *game->currentLayout), game->components.total_tileComponents);

	//void** comps = ECS_getEntity(*game->currentLayout, 1);
	//Position* poss = (Position*)comps[POSITION];
	//Sprite* sprite = (Sprite*)comps[SPRITE];


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

	//// deselect everything if nothing was clicked
	//int x, y;
	//Uint32 buttons;
	//SDL_PumpEvents();
	//buttons = SDL_GetMouseState(&x, &y);
	//if ((buttons & SDL_BUTTON_LMASK) != 0) {
	//	bool clickedNothing = true;
	//	for (int i = 0; i < game->components.total_spriteComponents; i++) {
	//		if (Sprite_isPointInside(&game->components, &game->components.spriteComponents[i], (Vec2) { x, y }))
	//			clickedNothing = false;
	//	}
	//	for (int i = 0; i < game->components.total_tileComponents; i++) {
	//		if (Tile_isPointInside(&game->components, &game->components.tileComponents[i], (Vec2) { x, y }))
	//			clickedNothing = false;
	//	}
	//	if (clickedNothing)
	//		Editor_deselectAll(game->currentLayout);
	//}

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
				CollisionBox* box = ECS_getNthComponent(COLLISION_BOX, game->currentLayout, i);
				if (NULL == box) continue;
				if (CollisionBox_isPointInside(game->currentLayout, box, a) || CollisionBox_isPointInside(game->currentLayout, box, b)) isOnFloor = true;
			}
			if (isOnFloor)
				((PhysicsBody*)mario[PHYSICS_BODY])->velocity.y = -800;
		}
		ECS_freeEntity(mario);
	}

	// update components
	if (game->state == EDIT_MODE) {
		for (int i = 0; i < game->components.total_spriteComponents; i++)
			Sprite_edit_update(&game->components, &game->components.spriteComponents[i], game->renderer);
		for (int i = 0; i < game->components.total_tileComponents; i++)
			Tile_edit_update(&game->components, &game->components.tileComponents[i], game->renderer);
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
		// serialise_components
		//Position_serialise(game->components.positionComponents, 128, "./saves/position.data");
		//Sprite_serialise(game->components.spriteComponents, 128, "./saves/sprite.data");
		//Editor_serialise(game->components.editorComponents, 128, "./saves/editor.data");
		//Animation_serialise(game->components.animationComponents, 128, "./saves/animation.data");
		//Tile_serialise(game->components.tileComponents, 128, "./saves/tile.data");
		//Text_serialise(game->components.textComponents, 128, "./saves/text.data");
		//CollisionBox_serialise(game->components.collisionBoxComponents, 128, "./saves/collisionBox.data");
		//Collider_serialise(game->components.colliderComponents, 128, "./saves/collider.data");
		//PhysicsBody_serialise(game->components.physicsBodyComponents, 128, "./saves/physicsBody.data");
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
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.x = (tile->size.x > 1) ? tile->size.x - 1 : 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.x = (tile->tilePosition.x > 0) ? tile->tilePosition.x - 1 : 0;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(game->currentLayout)), (Vec2) { -12, 0 });
				}
			}
			break;
		case SDLK_UP:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.y = (tile->size.y > 1) ? tile->size.y - 1 : 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.y = (tile->tilePosition.y > 0) ? tile->tilePosition.y - 1 : 0;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(game->currentLayout)),(Vec2) { 0, -12 });
				}
			}
			break;
		case SDLK_RIGHT:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.x = tile->size.x + 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.x = tile->tilePosition.x + 1;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(game->currentLayout)), (Vec2) { 12, 0 });
				}
			}
			break;
		case SDLK_DOWN:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->size.y = tile->size.y + 1;
				}
				else if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LALT] != 0) {
					Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
					if (NULL != tile) tile->tilePosition.y = tile->tilePosition.y + 1;
				}
				else {
					Position_moveBy(ECS_getPositionComponent(&game->components, Editor_getSelected(game->currentLayout)), (Vec2) { 0, 12 });
				}
			}
			break;
		case SDLK_c:
			if (game->state == EDIT_MODE) {
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LCTRL] != 0) {
					Editor_copy(&game->components, Editor_getSelected(game->currentLayout));
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
				ECS_deleteEntity(&game->components, Editor_getSelected(game->currentLayout));
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

	if (game->state == EDIT_MODE) {
		Tile* tile = ECS_getTileComponent(&game->components, Editor_getSelected(game->currentLayout));
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
