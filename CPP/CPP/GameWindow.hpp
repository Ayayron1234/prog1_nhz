#pragma once
#include <string>
#include "Vec2.h"
#include "misc.hpp"

#include <SDL.h>
#undef main
#include "SDL_ttf.h"

class GameWindow {
private:
	RGBColor backgroundColor;
	bool isFullscreen;
	SDL_Window* window;
	std::string name;

	void SDL_init() {
		int flags = SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC;
		flags = (this->isFullscreen) ? flags | SDL_WINDOW_FULLSCREEN : flags;

		// Initialise SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			SDL_Log("SDL cound not be initialised: %s\n", SDL_GetError());
			return;
		}

		// Create window
		this->window = SDL_CreateWindow(this->name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			this->size.x, this->size.y, flags);
		if (this->window == NULL)
		{
			SDL_Log("SDL couldn't open %s window: %s\n", this->name.c_str(), SDL_GetError());
			return;
		}

		// Create rendering context
		this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_SOFTWARE);
		if (this->renderer == NULL)
		{
			SDL_Log("SDL couldn't create rendering context: %s", SDL_GetError());
			return;
		}
		SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
		SDL_RenderClear(this->renderer);
		
		// Init TTF
		if (TTF_Init() == -1) {
			printf("TTF_Init: %s\n", TTF_GetError());
			exit(2);
		}
	}

	void HandleSDLEvents() {
		SDL_Event event;
		SDL_PollEvent(&event);

		switch (event.type)
		{
		case SDL_QUIT:
			this->isRunning = false;
			break;
		default:
			break;
		}
	}
public:
	Vec2Int size;

	SDL_Renderer* renderer;

	bool isRunning;

	GameWindow(const std::string name = "Game Window", Vec2Int size = Vec2Int(720, 480), RGBColor backgroundColor = {0, 0, 0}) {
		this->name = name;
		this->size = size;
		this->isRunning = true;

		this->SetBackgroundColor(0, 0, 0);
		this->SDL_init();
	}

	void SetBackgroundColor(int R, int G, int B) {
		this->backgroundColor = { R, G, B };
	}
	void SetBackgroundColor(RGBColor color) {
		this->backgroundColor = color;
	}

	void Render() {
		this->HandleSDLEvents();

		SDL_SetRenderDrawColor(this->renderer, this->backgroundColor.r, this->backgroundColor.g, this->backgroundColor.b, 255);
		SDL_RenderClear(this->renderer);

		SDL_RenderPresent(this->renderer);
	}

	void Quit() {
		this->isRunning = false;
	}
};
