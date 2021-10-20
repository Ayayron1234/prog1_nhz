#ifndef GAME_H
#define GAME_H

#include <math.h>
#include <time.h>
#include <SDL_image.h>
#include <stdlib.h>
#include "Texture.h"
#include "ECS.h"


typedef struct RGBAColor {
	int r;
	int g;
	int b;
	int a;
} RGBAColor;

typedef struct Game
{
	bool isLoaded;

	ComponentLists components;
	int ENTITIES;

	bool isRunning;
	SDL_Window* window;
	SDL_Surface* windowSurface;
	SDL_Renderer* renderer;
	RGBAColor backgroundColor;
} Game;

Game initGame(char *windowName, Vec2 windowDimentions, bool isFullScreen, RGBAColor backgroundColor);
void updateGame(Game *game);
void renderGameElements(Game* game);

void handleSDLEvents(Game *game);
void quitGame(Game *game);

#endif
