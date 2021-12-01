#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <string.h>
#include "SDL.h"
#include "misc.h"
#include "Math.h"
#include "Tilemap.h"


typedef struct Text {
	int ENTITY_ID;

	char value[255];
	int fontSize;
	char fontFamily[255];
	RGBColor fontColor;
	Vec2Int textBoxSize;

	RenderProperties renderProps;
} Text;

#endif // !TEXT_H
