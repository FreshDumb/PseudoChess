#include "Game/Game.h"
#include <sstream>

int main(int argc, char* argv[]) // ACHTUNG: (int, char **) nicht vergessen!
{

	bool Running = true;
	Game game = Game();

	int Width = WINDOWWIDTH;
	int Height = TILESIZE * 11;

	if (SDL_Init(SDL_INIT_VIDEO) == 0) {
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		if (SDL_CreateWindowAndRenderer(Width, Height, 0, &window, &renderer) == 0) {
			game.initGame(renderer, window);

			while (game.gameQuit() == SDL_FALSE) {
				game.update();
			}
		}
		SDL_DestroyWindow(window);
	}

	void TTF_Quit();
	void SDL_Quit();
	return 0;
}

