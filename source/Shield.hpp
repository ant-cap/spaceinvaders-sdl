#ifndef shield_hpp
#define shield_hpp

#include "SDL.h"
#include "Utility.hpp"

class Shield
{
private:
	SDL_Rect srcRect, dstRect;
public:
	Shield(SDL_Rect* dstR);
	~Shield() {}

	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);
	bool isWithin(XY* point);
};

#endif
