#include "InputManager.h"

void InputManager::pollEvents() {
	while (SDL_PollEvent(&Event)) {
		switch (Event.type)
		{
		case SDL_QUIT:
			quit = SDL_TRUE;
			break;
		case SDL_KEYDOWN:
			if (Event.key.keysym.sym == SDLK_RETURN) {
				enter = true;
			}
			break;
		case SDL_KEYUP:
			if (Event.key.keysym.sym == SDLK_RETURN) {
				enter = false;
			}
			break;
		case SDL_MOUSEMOTION:
			mousePos = Vector2(Event.motion.x, Event.motion.y);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (Event.button.button == SDL_BUTTON_LEFT) {
				leftButtonDown = true;
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if (Event.button.button == SDL_BUTTON_LEFT) {
				leftButtonDown = false;
			}
			break;
		}
	}
}
