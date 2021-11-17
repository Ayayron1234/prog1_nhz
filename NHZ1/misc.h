#pragma once
#include "Tilemap.h"
#include "SDL_ttf.h"

typedef struct RGBAColor {
	int r;
	int g;
	int b;
	int a;
} RGBAColor;

typedef struct RGBColor {
	int r;
	int g;
	int b;
} RGBColor;

typedef enum GameState {
	GAME_MODE = 0,
	EDIT_MODE = 1
} GameState;

typedef struct GameResources {
	Tilemap tilemap;
	TTF_Font* mainFont;
} GameResources;
