#include "Text.h"

void Text_init(Text* texts, int entityID, int* total_textComponents, char value[255], int fontSize, char fontFamily[255], RGBColor fontColor, Vec2Int textBoxSize)
{
	Text_delete(texts, entityID, total_textComponents);
	texts[*total_textComponents].ENTITY_ID = entityID;
	strcpy_s(texts[*total_textComponents].value, 255, value);
	texts[*total_textComponents].fontSize = fontSize;
	strcpy_s(texts[*total_textComponents].fontFamily, 255, fontFamily);
	texts[*total_textComponents].fontColor = fontColor;
	texts[*total_textComponents].renderProps = (RenderProperties){ .isVisible = true, .opacity = 1.0, .scale = 1.0 };
	texts[(*total_textComponents)++].textBoxSize = textBoxSize;
}

void Text_delete(Text* texts, int entityID, int* total_textComponents) {
	bool found = false;
	for (int i = 0; i < *total_textComponents; i++) {
		if (!found && texts[i].ENTITY_ID == entityID) 
			found = true;
		if (found) {
			texts[i] = (i < *total_textComponents - 1) ? texts[i + 1] : (Text) { 0 };
			if (i < *total_textComponents - 1) {
				strcpy_s(texts[i].value, 255, texts[i+1].value);
				strcpy_s(texts[i].fontFamily, 255, texts[i + 1].fontFamily);
			}
		}
	}
	if (found) (*total_textComponents)--;
}

void Text_deserialise(Text* texts, int* total_textComponents, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Loading %d Text components from file: %s...", maxNumberOfComponents, path);
	Text* loadedTexts;
	loadedTexts = (Text*)calloc(maxNumberOfComponents, sizeof(Text));
	if (NULL == loadedTexts) exit(1);

	FILE* file;
	fopen_s(&file, path, "rb");
	if (file != 0) {
		fread(loadedTexts, sizeof(Text), maxNumberOfComponents, file);
		*total_textComponents = 0;
	}

	for (int i = 0; i < maxNumberOfComponents; i++) {
		if (loadedTexts[i].ENTITY_ID != 0)
			*total_textComponents += 1;
		else break;
		texts[i] = loadedTexts[i];
		strcpy_s(texts[i].value, 255, loadedTexts[i].value);
		strcpy_s(texts[i].fontFamily, 255, loadedTexts[i].fontFamily);
	}
	free(loadedTexts);
	if (file != 0) fclose(file);
}

void Text_serialise(Text* texts, int maxNumberOfComponents, char path[255])
{
	SDL_Log("Saving %d Text components to file: %s...", maxNumberOfComponents, path);

	FILE* file;
	fopen_s(&file, path, "wb");
	if (NULL == file) exit(1);
	if (file != 0) {
		fwrite(texts, sizeof(Text), maxNumberOfComponents, file);
		fclose(file);
	}
}
