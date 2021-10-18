#include "Game.h"
#include "Vec2.h"

int main(int argc, char* argv[])
{
	RGBAColor gameBackground = { .r = 3, .g = 102, .b = 252, .a = 255 };
	Vec2 gameDimentions = {720, 480};
	Game game = initGame("main window", gameDimentions, false, gameBackground);


	while (game.isRunning)
	{
		handleSDLEvents(&game);
		updateGame(&game);
		renderGameElements(&game);
	}

	quitGame(&game);
	return 0;
}
//#include <SDL.h>
//#include <SDL_image.h>
//#include <stdlib.h>
//
//
///* mezon allo figura. ugyanolyan sorrendben vannak, mint a kepen,
// * igy a kapott egesz szamok megegyeznek a png-beli indexekkel */
//typedef enum Babu {
//    VKiraly, VVezer, VBastya, VFuto, VHuszar, VGyalog,
//    SKiraly, SVezer, SSastya, SFuto, SHuszar, SGyalog
//} Babu;
//
///* a pieces.png fajlban levo figurak merete */
//enum { MERET = 520 };
//
///* kirajzol egy babut; a forras a betoltott png, a cel nevu kepre rajzol.
// * melyik babut, milyen koordinatakra: melyik, x, y. */
//void babu_rajzol(SDL_Renderer* renderer, SDL_Texture* babuk, Babu melyik, int x, int y) {
//    /* a forras kepbol ezekrol a koordinatakrol, ilyen meretu reszletet masolunk. */
//    SDL_Rect src = { (melyik % 6) * 62 + 10, (melyik / 6) * 60 + 10, MERET, MERET };
//    /* a cel kepre, ezekre a koordinatakra masoljuk */
//    SDL_Rect dest = { x, y, MERET, MERET };
//    /* kepreszlet masolasa */
//    SDL_RenderCopy(renderer, babuk, NULL, &dest);
//}
//
//
///* ablak megnyitasa */
//void sdl_init(int szeles, int magas, SDL_Window** pwindow, SDL_Renderer** prenderer) {
//    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
//        SDL_Log("Nem indithato az SDL: %s", SDL_GetError());
//        exit(1);
//    }
//    SDL_Window* window = SDL_CreateWindow("SDL peldaprogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, szeles, magas, 0);
//    if (window == NULL) {
//        SDL_Log("Nem hozhato letre az ablak: %s", SDL_GetError());
//        exit(1);
//    }
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
//    if (renderer == NULL) {
//        SDL_Log("Nem hozhato letre a megjelenito: %s", SDL_GetError());
//        exit(1);
//    }
//    SDL_RenderClear(renderer);
//
//    *pwindow = window;
//    *prenderer = renderer;
//}
//
//
//int main(int argc, char* argv[]) {
//    SDL_Window* window;
//    SDL_Renderer* renderer;
//    sdl_init(320, 200, &window, &renderer);
//
//    /* kep betoltese */
//    SDL_Texture* babuk = IMG_LoadTexture(renderer, "izometric_tree.png");
//    if (babuk == NULL) {
//        SDL_Log("Nem nyithato meg a kepfajl: %s", IMG_GetError());
//        exit(1);
//    }
//
//    /* rajz keszitese */
//    babu_rajzol(renderer, babuk, VKiraly, 82, 74);
//    babu_rajzol(renderer, babuk, SGyalog, 82 + MERET, 74);
//    babu_rajzol(renderer, babuk, VHuszar, 82 + 2 * MERET, 74);
//    SDL_RenderPresent(renderer);
//
//    /* nincs mar ra szukseg: felszabaditjuk a memoriat */
//    SDL_DestroyTexture(babuk);
//
//    /* szokasos varakozas */
//    SDL_Event event;
//    while (SDL_WaitEvent(&event) && event.type != SDL_QUIT) {
//    }
//
//    SDL_Quit();
//    return 0;
//}