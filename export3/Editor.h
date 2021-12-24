#ifndef EDITOR_H
#define EDITOR_H

#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"


/**
Enables the user to edit entities. 
*/
typedef struct Editor {
	int ENTITY_ID;

	bool isSelected;
	bool copied;
} Editor;

#endif // !EDITOR_H

