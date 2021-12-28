[[prog1_nzh_devdocs|Dokumentáció]]

## kérdések
- Debugmalloc hibásan szokott lefutni
## változások a követelmény specifikációhoz képest
- A mentett file-ok egy egy mappába kerülnek. Ennek oka az hogy játékosonként már csak egy file-ra lesz szükség, ezért felesleges lenne őket külön mappába helyenzi. 

## implementációk
- [ ] ECS type that includes layouts, currentLayout, playername, ...
- [ ] versioning
	- [ ] blueprints
	- [ ] save blueprints
- [ ] interactables should use generic algorithms
- [ ] entityrenderer holds textures
- [x] kamera *(req: map, layer)* 1h
- [x] map
- [x] save to one file
- [x] layer 1h
- [x] főmenü *(req: kamera)* 3h
- [x] mentés több személyre *(req: főmenü)* 
- [x] collision box
- [x] collider *(req: collision box)*
- [x] physics body
- [x] platformer *(req: aabb)*
- [x] ellenség *(req: aabb, platformer)*
- [x] health *(req: platformer)*
- [x] pickups *(req: platformer)*
- [ ] leaderboard *(req: kamera, főmenü)*

## fix
- [x] font family-t minden frame-ben betölti a játék
- [x] change `#pragma once` to `#ifndef ...`
- [ ] some ecs  functions expect `Layout` instead of `*Layout` as parameter
- [ ] render order is not based on the layer
- [x] switch player when in edit mode

## bugs
- [x] texts can be reselected in each frame




### layers
```c
enum RenderFilter { EDIT_MODE, GAME_MODE }

struct EntityRenderer {
	int layerIndex;
	RenderFilter filter;
}
```

### layouts
```c
typedef struct Layout {
	ComponentLists components;   // components of the layout
	Layer* layers;				 // list of layers
	Vec2 camera;   				 // the camera's position
} Layout;
```
game should have an array of layouts
```c
typedef struct LayoutMap {
	int start;
	int end;
} LayoutMap;

typedef struct SerialisationMapFragment {
	char componentType[255];     // eg.: Position, Sprite, ...
	int total_components;   	 // number of components of a certain type
	size_t componentSize		 // sizeof(ComponentTypeX)
	
	int total_layouts;		     // number of layouts
	LayoutMap* layoutMaps;       // array of serialised layouts
} SerialisationMapFragment;
```

```c
ComponentType ECS_getComponents(Layout currentLayout, char componentType[255]);
	// eg.:
	Position pos = ECS_getComponents(Game_getCurrentLayout(), "position");
```

## daily tasks
- [x] **11.20**
	Full integration of new data structure
	- [x] ability to create new component
	- [x] ability to create new layout
	- [x] fix editor copy and paste
	- [x] deselect entities in edit mode when clicking on air
	- [x] save layout data
---
- [x] **11.21**
Fixing memory leaks with debugmalloc
- [x] **11.22**
	- [x] fix entity delete
	- [x] layer
	- [x] mentés tobb személyre
- [x] **11.23**
	Character controller component
	- [x] player controller
	- [x] goomba
	- [x] health
- [x] **11.24**
	Interactables (probably 1 component)
	- [x] buttons
	- [x] interactible colliders
	- [x] pickups
	- [x] főmenü (ha meg lesz a gomb, akkor konnyu)