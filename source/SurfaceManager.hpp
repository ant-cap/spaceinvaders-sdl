#ifndef surface_hpp
#define surface_hpp

#include "SDL.h"

class Game;

class SurfaceManager
{
private:
	Game* game;
	SDL_Window* window;
	SDL_Surface* screen;
	SDL_Surface* gameSurface;
	SDL_Surface* spritesheet;
	SDL_Surface* colormap;
public:
	SurfaceManager(Game* g, SDL_Window* gwindow);
	~SurfaceManager() {}

	void draw();
};

#endif