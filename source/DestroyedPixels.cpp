#include "DestroyedPixels.hpp"

void DestroyedPixels::draw(SDL_Surface* screen)
{
	SDL_LockSurface(screen);
	SDL_PixelFormat* fmt = screen->format;
	for (const int& i: indexes) {
		Uint32* p = (Uint32*)screen->pixels + i;
		*p = SDL_MapRGB(fmt, 0, 0, 0);
	}
	SDL_UnlockSurface(screen);
}
