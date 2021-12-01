#ifndef EDITORSYSTEMS_H
#define EDITORSYSTEMS_H

#include <stdbool.h>
#include "ECS.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include "CollisionBoxSystems.h"


/**
 * Renders a box around the selected entity if the game state is EDIT_MODE.
 * @param gameState The game's state.
 * @param currentLayout The layout where we want to display the box, and the parent layout of the selected entity. 
 * @param editor The editor we want to render.
 * @param renderer The rendering context where we want to render the box. 
 * @param ttfFont A pointer to the loaded font family which we want to use to display data about the entity. 
 */
void Editor_render(int gameState, Layout* currentLayout, Editor* editor, SDL_Renderer* renderer, TTF_Font* ttfFont);

/**
 * Renders position and layout data, and displays the entity's ID.
 * @param currentLayout The layout where we want to display the box, and the parent layout of the selected entity.
 * @param editor The editor we want to render.
 * @param renderer The rendering context where we want to render the box.
 * @param ttfFont A pointer to the loaded font family which we want to use to display data about the entity.
 */
void Editor_renderPositionData(Layout* currentLayout, Editor* editor, SDL_Renderer* renderer, TTF_Font* ttfFont);

/**
 * Returns the selected entity's ID.
 * @param currentLayout The layout where we want to look for the selected entity. 
 * @returns The entity's ID.
 */
int Editor_getSelected(Layout* currentLayout);

/**
 * Selects an entity with a given ID.
 * @param currentLayout The layout were we want to select the entity. 
 * @param entityID The ID of the entity we want to select. 
 */
void Editor_select(Layout* currentLayout, int entityID);

/**
 * Deselects all entities.
 * @param currentLayout The layout whos child entities we want to deselect. 
 */
void Editor_deselectAll(Layout* currentLayout);

/**
 * Selects entity at a given location.
 * @param currentLayout The layout where we want to select the entity. 
 * @param mousePosition The cursor's position relative to the game window. 
 */
void Editor_selectEntityAtClick(Layout* currentLayout, Vec2 mousePosition);

/**
 * Sets the copied attribute of the given entity to true.
 * @param currentLayout The layout where we want to copy the entity from. 
 * @param entityID The ID of the entity we want to copy.
 */
void Editor_copy(Layout* currentLayout, int entityID);

/**
 * Duplicates the copied entity.
 * @param layouts List of layouts.
 * @param numberOFlayouts The number of layouts.
 * @param layoutName The name of the layout where we want to paste the new enity in. 
 * @param copied The list of components that belong to the copied entity. 
 * @returns The ID of the new component. 
 */
int Editor_paste(Layout* layouts, int numberOfLayouts, char* layoutName, void** copied);

#endif // !EDITORSYSTEMS_H
