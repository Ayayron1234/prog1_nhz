#pragma once

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

enum gameState {
	GAME_MODE = 0,
	EDIT_MODE = 1
};
