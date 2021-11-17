#pragma once
#include <stdbool.h>
#include <SDL.h>
#include "SDL_ttf.h"
#include <string.h>
#include "ECS.h"
#include "misc.h"
#include "Math.h"
#include "SpriteSystems.h"
#include "PositionSystems.h"
#include "AnimationSystems.h"
#include "Tile.h"
#include "TileSystems.h"
#include "TextSystems.h"
#include "CollisionBoxSystems.h"
#include "PhysicsBodySystems.h"
#include "debugmalloc.h"


typedef struct GameAppearance {
	bool isFullScrean;

	char windowName[255];
	Vec2Int windowDimensions;
	RGBAColor backgroundColor;
} GameAppearance;

typedef struct GameTime {
	long int lastUpdateTime;
	long int deltaT;
} GameTime;

/**
This is the main structure. All game data is stored in this object. 
*/
typedef struct Game
{
	ComponentLists components;

	int numberOfLayouts;
	Layout* layouts;
	Layout* currentLayout;
	void** componentLists;

	GameResources resources;

	SDL_Window* window;
	SDL_Surface* windowSurface;
	SDL_Renderer* renderer;

	int state;
	bool isRunning;
	GameAppearance appearance;
	Tilemap tilemap;
	GameTime time;
} Game;

/**
* Initialises the game. Loads components from file, initialises sdl.
* @param game A pointer to the game object we want to initialise. 
* @param windowName The name of the window in wich the game will render. 
* @param windowDimensions Width and height of the game window.
* @param isFullScreen true if game should open in fullscreen mode. 
* @param backgroundColor rgba value of the background color of the game window. 
*/
void Game_init(Game *game, char windowName[255], Vec2Int windowDimensions, bool isFullScreen, RGBAColor backgroundColor);

/**
* Initialises sdl. 
* @param game A pointer to the game object. The function will store pointers to the renderer and window in this object. 
*/
void Game_sdlInit(Game* game);

/**
* Updates components in the current layout
* @param layout The layout in which the components should be updated. 
*/
void Game_update(Game* game);

/**
* Handle sdl events, like the quit event. 
*/
void Game_handleSDLEvents(Game* game);

/**
* Renders components in the current layout
* @param layout The layout in which the components should be rendered.
*/
void Game_renderElements(Game* game);

