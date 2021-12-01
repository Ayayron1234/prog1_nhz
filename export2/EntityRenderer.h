#ifndef ENTITYRENDERER_H
#define ENTITYRENDERER_H


typedef enum RenderFilter { RENDER_FILTER_ALLWAYS = 0, RENDER_FILTER_EDIT = 1, RENDER_FILTER_GAME = 2 } RenderFilter;

typedef struct EntityRenderer {
	int ENTITY_ID;

	int layerIndex;
	RenderFilter filter;
} EntityRenderer;

#endif // !ENTITYRENDERER_H
