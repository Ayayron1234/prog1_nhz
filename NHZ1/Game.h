#pragma once
#include <stdbool.h>
#include <SDL.h>
#include "ECS.h"
#include "Position.h"
#include "misc.h"

typedef struct Game
{
	ComponentLists *components;
	int ENTITIES;

	bool isRunning;
	SDL_Window* window;
	SDL_Surface* windowSurface;
	SDL_Renderer* renderer;
	RGBAColor backgroundColor;
} Game;

Game Game_init(char* windowName, Vec2 windowDimentions, bool isFullScreen, RGBAColor backgroundColor);
