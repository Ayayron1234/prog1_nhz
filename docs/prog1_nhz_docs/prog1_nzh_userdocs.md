# Felhasználói dokumentáció
### Kezdőképernyő
A kezdőképernyőn kell megadni a játékos nevét. Ha olyan játékosnak a nevét adjuk meg aki már elkezdte a játékot, de még nem ért el a célba, akkor a PLAY gomb kijelölésével és enter megnyomásával, a korábbi helyzetéből folytathatja a játékot, egyébként előlről kezdődik. 
### Játék
A játékos a karaktert a jobbra és a ballra nyilakkal tudja irányítani, és a felfele nyíl megnyomásával tud ugrani. 
#### Játékelemek:
- **Goomba**
	![[Pasted image 20211128144048.png]]
	ha a játékos felülről érinti, a goomba eltűnik, ha viszont oldalról, akkor a játékos veszít egy életet. 
- .
	![[Pasted image 20211128144021.png]]
	Ha a játékos alulról érinti akkor két esemény lehetséges:
	1. A játékos kap egy coin-t. ez a score-számolásában játszik szerepet.
	2. Az érintett tile fölött megjelenik egy mushroom
- **Mushroom**
	![[Pasted image 20211128144101.png]]
	Ha a játékos érinti, nő a játékos életeinek száma
- **Flag**
![[Pasted image 20211128144114.png]]
	Ha a játékos érinti, végetér a játék, és a saves mappába elmentődik a játékos neve, és a játékos által elért score. 
	
### Map editor
A map editor-ba az **F5** megnyomásával lehet elérni.
- **Bal klikk** egy entity-n kijelöli azt. 
- A kijelölt entity a **nyilak**kal mozgatható
- **Shift+nyilak**kal lehet változtatni, hogy az entity melyik layer-en jelenjen meg.
- **Space** megnyomásával lehet layout-ok között váltani
- **Ctrl+c** megnyomásával lehet másolásra megjelölni egy entity-t
- ** Ctrl+v** megnyomásával lehet beilleszteni a másolt entity-t a kijelölt entity mellé. 
- Ha a kijelölt entity egy tile entity, **Ctrl+nyilak**kal lehet állítani a méretét. 
- Ha a kijelölt entity egy tile entity, **Alt+nyilak**kal lehet állítani a textúráját.
- Az **F5** újbóli megnyomásával lehet visszaváltani játék-módba