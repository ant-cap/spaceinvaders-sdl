/*
SurfaceManager
	Handles the rendering of color on to the game surface.
	Colors are grabbed from colormap.bmp (orig.) or colormap2.bmp (color ver.)
*/

#include "SurfaceManager.hpp"
#include "Game.hpp"

SurfaceManager::SurfaceManager(Game* g, SDL_Window* gwindow)
{
	game = g; window = gwindow;
	screen = SDL_GetWindowSurface(window);
	gameSurface = SDL_CreateRGBSurface(0, 224, 256, 32, 0, 0, 0, 0);
	spritesheet = SDL_LoadBMP("assets/spritesheet.bmp");
	colormap = SDL_LoadBMP("assets/colormap.bmp");
}

void SurfaceManager::draw()
{
	SDL_FillRect(gameSurface, NULL, SDL_MapRGB(gameSurface->format, 0, 0, 0));
	game->drawGame(gameSurface, spritesheet);

	// work on getting the color
	SDL_LockSurface(gameSurface);
	SDL_LockSurface(colormap);
	SDL_PixelFormat* fmt = gameSurface->format;
	Uint8 r, g, b; int size = gameSurface->w * gameSurface->h, y = 0, x = 0, colorindex = 0;
	for (int i = 0; i < size; i++) {
		Uint32* pixel = (Uint32*)gameSurface->pixels + i;
		SDL_GetRGB(*pixel, fmt, &r, &g, &b);
		if (r == 0) continue;
		y = int(i / gameSurface->w); x = i % gameSurface->w;
		//printf("Pixel drawn at %d %d\n", x, y);
		colorindex = (int(y/8) * colormap->w) + (int(x / 8));
		//printf("Colorindex: %d\n", colorindex);
		Uint32* color = (Uint32*)colormap->pixels + colorindex;
		SDL_GetRGB(*color, colormap->format, &r, &g, &b);
		*pixel = SDL_MapRGB(fmt, r, g, b);
	}
	SDL_UnlockSurface(colormap);
	SDL_UnlockSurface(gameSurface);

	// draw game on screen (scaled *3)
	SDL_BlitScaled(gameSurface, NULL, screen, NULL);
	SDL_UpdateWindowSurface(window);
}
