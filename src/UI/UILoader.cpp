#include"UILoader.h"

UILoader::UILoader() {

}

UILoader::~UILoader() {

}

void UILoader::loadTexturesToSurface() {
	UnitSurf[0] = SDL_LoadBMP("dat/Units/Pawn.bmp");
	UnitSurf[1] = SDL_LoadBMP("dat/Units/Rogue.bmp");
	UnitSurf[2] = SDL_LoadBMP("dat/Units/Rook.bmp");
	UnitSurf[3] = SDL_LoadBMP("dat/Units/King.bmp");
	UnitSurf[4] = SDL_LoadBMP("dat/Units/Bishop.bmp");
	TargetSurf = SDL_LoadBMP("dat/Target.bmp");
	PickupSurf[0] = SDL_LoadBMP("dat/Pickups/Coin.bmp");
	std::cout << SDL_GetError() << "\n";
}

void UILoader::loadFont(const char* _fontLocation) {
	font = TTF_OpenFont(_fontLocation, 16);
}

SDL_Surface* UILoader::getTextSurface(const char* _text) {
	SDL_Color White = { 0, 0, 0 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, _text, White);
	return surfaceMessage;
}