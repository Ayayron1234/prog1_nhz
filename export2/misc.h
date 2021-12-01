#ifndef MISC_H
#define MISC_H

#include "Tilemap.h"
#include "SDL_ttf.h"


/**
RGB color values with an alpha channel. 
 */
typedef struct RGBAColor {
	int r;					///< red channel
	int g;					///< green channel			
	int b;					///< blue channel
	int a;					///< alpha channel
} RGBAColor;

/**
RGB color values
 */
typedef struct RGBColor {
	int r;					///< red channel
	int g;					///< green channel
	int b;					///< blue channel
} RGBColor;

/**
State of the game. Some game logic works differently depending what state the game is in. 
 */
typedef enum GameState {
	GAME_MODE = 0,
	EDIT_MODE = 1,
	VICTORY = 2
} GameState;

/**
Outside data that is used by the game. This is used when loading game objects, because than the pointers have to be reset. 
 */
typedef struct GameResources {
	Tilemap tilemap;		///< A tilemap object. 
	TTF_Font* mainFont;		///< The font family used by the game. 
} GameResources;

#endif // !MISC_H

