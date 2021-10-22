#pragma once
#include <stdbool.h>
#include <SDL.h>
#include <string.h>
#include "ECS.h"
#include "misc.h"
#include "SpriteSystems.h"
#include "PositionSystems.h"
#include "AnimationSystems.h"
#include "Tile.h"
#include "TileSystems.h"


typedef struct GameAppearance {
	bool isFullScrean;

	char windowName[255];
	Vec2Int windowDimensions;
	RGBAColor backgroundColor;
} GameAppearance;

typedef struct Game
{
	ComponentLists components;
	int ENTITIES;

	SDL_Window* window;
	SDL_Surface* windowSurface;
	SDL_Renderer* renderer;

	bool isRunning;
	GameAppearance appearance;
	Tilemap tilemap;
} Game;

void Game_init(Game *game, char windowName[255], Vec2Int windowDimensions, bool isFullScreen, RGBAColor backgroundColor);
void Game_sdlInit(Game* game);

void Game_update(Game* game);
void Game_handleSDLEvents(Game* game);
void Game_renderElements(Game* game);

