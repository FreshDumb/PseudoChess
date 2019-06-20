#pragma once
#include "../Color.h"
#include <SDL_ttf.h>

class UILoader {
public:
	SDL_Surface* UnitSurf[5];
	SDL_Surface* PickupSurf[1];
	SDL_Surface* TargetSurf;
	TTF_Font* font;

	UILoader();
	~UILoader();
	void loadTexturesToSurface();
	void loadFont(const char* _fontLocation);
	SDL_Surface* getTextSurface(const char* _text);
};