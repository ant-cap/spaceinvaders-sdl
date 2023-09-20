#ifndef invader_hpp
#define invader_hpp

#include "SDL.h"
#include "Utility.hpp"

class Invader
{
private:
	SDL_Rect srcRect, dstRect;
	uint8_t type; bool active;
public:
	Invader(uint8_t t, SDL_Rect* d);
	~Invader() {}

	// draw will return true if pixels need to be "destroyed"
	bool draw(SDL_Surface* surface, SDL_Surface* spritesheet);
	bool move(bool* direction, bool* down, bool* fast);
	void setActive(bool a) { active = a; }
	void swapSprite(bool dead);
	bool isActive() { return active; }
	bool isAtBorder(bool* dir) { return (*dir) ? (dstRect.x >= 200) : (dstRect.x <= 8); }
	bool isWithin(XY* point);

	SDL_Rect* getDRect() { return &dstRect; }
	XY laserSpawn() { return XY(dstRect.x + 7, dstRect.y + 15); }
};

#endif
