#ifndef GAME_H
#define GAME_H

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
#include "MovementControllerSystems.h"
#include "InteractableSystems.h"
#include "PositionSystems.h"
#include "HealthSystems.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>


typedef struct GameAppearance {
	bool isFullScrean;

	char windowName[255];
	Vec2Int windowDimensions;
	RGBAColor backgroundColor;
} GameAppearance;

typedef struct GameTime {
	long int lastUpdateTime;
	long int deltaT;
	long int timeSinceStart;
} GameTime;

/**
This is the main structure. All game data is stored in this object. 
game->componentLists
*/
typedef struct Game
{
	char player[255];

	int numberOfLayouts;
	Layout* layouts;
	Layout* currentLayout;

	GameResources resources;
	
	int state;
	bool isRunning;
	GameAppearance appearance;
	GameTime time;

	SDL_Window* window;
	SDL_Renderer* renderer;
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

void Game_setup(Game* game);

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

#endif // !GAME_H
