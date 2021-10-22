#include "Editor.h"


void Editor_init(Editor *editors, int entityID, int* total_editorComponents)
{
	editors[*total_editorComponents].ENTITY_ID = entityID;
	editors[(*total_editorComponents)++].isSelected = false;
}

void Editor_deserialise(Editor* editors, int* total_EditorComponents, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Loading %d Editor components from file: %s...", maxNumberOfComponents, path);
	Editor* loadedEditors;
	loadedEditors = (Editor*)calloc(maxNumberOfComponents, sizeof(Editor));
	if (NULL == loadedEditors) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedEditors, sizeof(Editor), maxNumberOfComponents, file);
		*total_EditorComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedEditors[i].ENTITY_ID != 0)
			*total_EditorComponents += 1;
		else break;
		editors[i] = loadedEditors[i];
		editors[i].isSelected = false;
	}
	free(loadedEditors);
	if (file != 0) fclose(file);
}

void Editor_serialise(Editor* editors, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Saving %d Editor components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(editors, sizeof(Editor), maxNumberOfComponents, file);
		fclose(file);
	}
}
