#include"Color.h"
Color::Color() {
	r = g = b = a = 0;
}

Color::Color(int _r, int _g, int _b, int _a) {
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}
Color::Color(int _r, int _g, int _b) {
	r = _r;
	g = _g;
	b = _b;
	a = 255;
}

Uint32 Color::ColourToUint(int R, int G, int B){
	return (Uint32)((R << 16) + (G << 8) + (B << 0));
}

SDL_Colour Color::UintToColour(Uint32 color)
{
	SDL_Colour tempcol;
	tempcol.a = 255;
	tempcol.r = (color >> 16) & 0xFF;
	tempcol.g = (color >> 8) & 0xFF;
	tempcol.b = color & 0xFF;
	return tempcol;
}

Uint32 Color::getpixel(SDL_Surface *surface, int x, int y)
{
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to retrieve */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;

	case 2:
		return *(Uint16 *)p;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;

	case 4:
		return *(Uint32 *)p;
		break;

	default:
		return 0;       /* shouldn't happen, but avoids warnings */
	}
}