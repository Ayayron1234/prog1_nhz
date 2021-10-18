#include <stdbool.h>

typedef struct Window {
	int width;
	int height;
	bool fullscreen;
	void (*open)(void);
} Window;

Window createWindow(char* name);