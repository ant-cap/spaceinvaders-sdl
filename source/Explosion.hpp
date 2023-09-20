#ifndef explosion_hpp
#define explosion_hpp

#include "SDL.h"
#include "Utility.hpp"

class DestroyedPixels;

class Explosion
{
private:
	DestroyedPixels* dp;
	SDL_Rect srcRect, dstRect;
	bool type, destroy;
	int timer; 
public:
	Explosion(XY* pos, bool t, DestroyedPixels* destroyedpixels);
	~Explosion() {}

	void update();
	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);
	bool* destroyMe() { return &destroy; }
};

#endif
