#pragma once
#include <stdbool.h>
#include "Vec2.h"


typedef struct Camera {
	int ENTITY_ID;
} Camera;

void Camera_init(Camera* cameras, int entityID, int* total_cameraComponents);
void Camera_delete(Camera* cameras, int entityID, int* total_cameraComponents);

void Camera_deserialise(Camera* cameras, int* total_cameraComponents, int maxNumberOfComponents, char path[255]);
void Camera_serialise(Camera* cameras, int maxNumberOfComponents, char path[255]);

/*
* 
* camera should also be an entity!!!
*	position component
*   + camera component
*	this way fixing the camera to the player would be as easy as adding the camera component to the player's entity
* 
* also probably keyboard and mouse should also be components
* 
component:
	EntityRenderer:
		params:
			pointer to layout
			layer_index (this can only be efficent if it's easy to change an entity's layer index)

misc objects:
	Layout:
		params:
			camera,
			list of layers
				(the z-index of the layer is it's position in the "layers" array)

	Layer:
		params:
			parallax,
			opacity,

	Camera:
		params:
			position
			zoom
		functions:
			transform_to_window(components, entity_renderer, position_vector) => transformed_vector
			transform_from_window(components, entity_renderer, position_vector) => transformed_vector


*/

//typedef struct Layout {
//
//};
//
//typedef struct Camera {
//	int CAMERA_ID;
//
//	Vec2 position;
//	double zoom;
//} Camera;
//
//typedef struct Layout {
//	int LAYOUT_ID;
//
//	bool isVisible;
//};
//
//
//typedef struct EntityRenderer {
//	int ENTITY_ID;
//	
//	int Layout;
//	Camera* camera;
//	int Layer;
//
//} EntityRenderer;