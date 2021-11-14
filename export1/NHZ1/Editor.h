#pragma once
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

/**
* Create Editor component in a given ComponentLists.
* @param editors The array in which the component should be created.
* @param entityID The ID of the component's parent entity.
* @param total_editorComponents A pointer to the variable which stores the number of components of this kind in it's parent layout.
*/
void Editor_init(Editor* editors, int entityID, int* total_editorComponents);

/**
* Deletes a given entity's Editor component.
* @param The array from which the component should be deleted.
* @param entityID The ID of the component's parent entity.
* @param total_editorComponents A pointer to the variable which stores the number of components of this kind in it's parent layout.
*/
void Editor_delete(Editor* editors, int entityID, int* total_editorComponents);

// temp
void Editor_deserialise(Editor* editors, int* total_EditorComponents, int maxNumberOfComponents, char path[255]);
void Editor_serialise(Editor* editors, int maxNumberOfComponents, char path[255]);
