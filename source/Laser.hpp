#ifndef laser_hpp
#define laser_hpp

#include "SDL.h"
#include "Utility.hpp"
#include <vector>
using std::vector;

class Cannon;
class Army;

class Laser
{
private:
	Cannon* cannon;
	Army* army;
	SDL_Rect srcRect, dstRect;
	int type, frame, wait;
	vector<int> frames;
	bool sender, collided;
	XY* colPos;
public:
	Laser(int t, XY* pos, Cannon* c, Army* a);
	~Laser() {}

	void update();
	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);

	void setCollided(XY* pos);
	bool* getSender() { return &sender; }
	bool waiting() { return (wait != 0); }
	bool isWithin(XY* point);
	SDL_Rect* getRect(bool ds) { return (ds) ? &dstRect : &srcRect; }
};

#endif
