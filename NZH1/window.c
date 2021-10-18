#include "window.h"
#include <SDL.h>
#include <stdbool.h>

Window createWindow(char* name) {
	Window win = { .fullscreen = false, .width = 600, .height = 400 };
	
	return win;
}