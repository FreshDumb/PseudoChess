#include "SDL.h"
#include <iostream>

class Color {
public:
	char r, g, b, a;
	Color();
	Color(int _r, int _g, int _b, int _a);
	Color(int _r, int _g, int _b);

	static Uint32 ColourToUint(int R, int G, int B);
	static SDL_Colour UintToColour(Uint32 color);

	// Getting the value of a pixel
	static Uint32 getpixel(SDL_Surface *surface, int x, int y);
};

