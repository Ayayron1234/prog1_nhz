[[prog1_nzh_devdocs|Dokumentáció]]

## kérdések

## változások a követelmény specifikációhoz képest
- A mentett file-ok egy egy mappába kerülnek. Ennek oka az hogy játékosonként már csak egy file-ra lesz szükség, ezért felesleges lenne őket külön mappába helyenzi. 

## implementációk
- [ ] kamera *(req: map, layer)*
- [ ] map
- [x] save to one file
- [ ] layer
- [ ] főmenü *(req: kamera)*
- [ ] mentés több személyre *(req: főmenü)*
- [x] collision box
- [x] collider *(req: collision box)*
- [x] physics body
- [x] platformer *(req: aabb)*
- [ ] ellenség *(req: aabb, platformer)*
- [ ] health *(req: platformer)*
- [ ] pickups *(req: platformer)*
- [ ] leaderboard *(req: kamera, főmenü)*

## fix
- [ ] font family-t minden frame-ben betölti a játék

## bugs
- [ ] texts can be reselected in each frame






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