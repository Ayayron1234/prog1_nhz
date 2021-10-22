#pragma once
#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"


typedef struct Editor {
	int ENTITY_ID;

	bool isSelected;
} Editor;

void Editor_init(Editor* editors, int entityID, int* total_editorComponents);
void Editor_deserialise(Editor* editors, int* total_EditorComponents, int maxNumberOfComponents, char path[255]);
void Editor_serialise(Editor* editors, int maxNumberOfComponents, char path[255]);
