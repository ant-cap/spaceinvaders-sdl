#ifndef cannon_hpp
#define cannon_hpp

#include "SDL.h"
#include "Laser.hpp"
#include "Utility.hpp"

class Game;

class Cannon
{
private:
	SDL_Rect srcRect, dstRect;
	XY firePos;
	Game* game;
	Laser* laser; bool freshlaser;
	int shotsFired, count;
	bool hit, sprite;
public:
	Cannon(Game* g);
	~Cannon() {}

	void update();
	void move(bool dir);
	void fire();
	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);
	void laserCollided(XY* pos);
	void callExplosion(XY* pos);
	void destroyLaser();
	void initHit();
	XY* getFirePos();
	Laser* getLaser() { return laser; }
	int* getShotsFired() { return &shotsFired; }
	bool* isHit() { return &hit; }
};

#endif
