#include <SDL.h>
#include "Board/Vector2.h"

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
class InputManager {
private:
	Vector2 mousePos;
	SDL_bool quit = SDL_FALSE;
	SDL_Event Event;
	bool leftButtonDown = false;


public:
	bool enter = false;

	void pollEvents();
	inline Vector2 getMouseAxis() { return mousePos; }
	inline bool getMouseButtonDown() { return leftButtonDown; }
	inline SDL_bool hasQuit() { return quit; }
};
#endif