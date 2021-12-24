A file-okban lévő függvényekről és struktúrákról a doxygen által generált dokumentációból, vagy a file-okban lévő comment-ekből lehet többet megtudni. 
## Komponenseket tároló file-ok
- CollisionBox.h
- Editor.h
- Interactable.h
- Position.h
- Health.h
- Collider.h
- MovementController.h
- PhysicsBody.h
- Animation.h
- EntityRenderer.h
- Sprite.h
- Text.h
- Tile.h
Ezekben a file-okban a különböző komponensek struct-jai vannak. 
A komponensek nagyrészéhet tartozik még 2 file. Ezeknek nevei a komponens neve Systems szóval a végén, és mindegyikhez van header file is. 
Tehát ilyen file hármasra példa: Position.h, PositionSytems.h, PositionSystems.c
Az ilyen ...Systems file-ok tartalmazzák az egyes komponensekhez tartozó függvényeket. 
## ECS.h, ECS.c
Ez a file adja a játék fő vezérlését. Itt vannak az Enity Component System megvalósításához- valamint a mentéshez és betöltéshez- szükséges függvények és struktúrák. 
## Math.h Math.c
Matematikával kapcsolatos függvények és struktúrák, pl.: vektormatek, min-max keresés stb. 
## misc.h
Olyan struktúrák, amiket több modul is használhat, de egyik másik file-ba se illettek volna. 
## Game.h, Game.c
A játékmenet függvényei találhatók itt. Ez a modul hívja meg a komponensek moduljainak függvényeit. 
## Tilemap.h, Tilemap.c
A tilemap struktúra és a hozzá tartozó függvények tartoznak ide. 
## main.c
Ugyan az mint minden programban. Itt van a while loop ami a játék fő ciklusaként működik. Innen indul a játék és itt szabadulnak fel a lefoglalt memóriaterületek a játék bezárását követve. 