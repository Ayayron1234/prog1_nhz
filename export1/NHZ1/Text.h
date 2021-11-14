#pragma once
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

void Text_init(Text* texts, int entityID, int* total_textComponents,
	char value[255], int fontSize, char fontFamily[255], RGBColor fontColor, Vec2Int textBoxSize);
void Text_delete(Text* texts, int entityID, int* total_textComponents);

void Text_deserialise(Text* texts, int* total_textComponents, int maxNumberOfComponents, char path[255]);
void Text_serialise(Text* texts, int maxNumberOfComponents, char path[255]);