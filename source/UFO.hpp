#ifndef ufo_hpp
#define ufo_hpp

#include "SDL.h"
#include "Utility.hpp"

class Game;
class UI;

class UFO
{
private:
	Game* game; UI* ui;
	SDL_Rect srcRect, dstRect;
	int explosion, wait, initFired; bool destroy, active, direction;
public:
	UFO(Game* g, UI* u);
	~UFO() {}

	void spawn();
	void update();
	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);
	void explode();

	bool* destroyMe() { return &destroy; }
	bool* isActive() { return &active; }
};

#endif
