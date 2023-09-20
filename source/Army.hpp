#ifndef army_hpp
#define army_hpp

#include "SDL.h"
#include "Utility.hpp"
#include "Invader.hpp"
#include "Laser.hpp"
#include <vector>
using std::vector;

class Game;
class DestroyedPixels;

class Army
{
private:
	Game* game;
	DestroyedPixels* dp;
	bool dir, down, frozen, moving, fast, queued, laserDestroyed;
	vector<vector<Invader*>> vaders;
	vector<Laser*> lasers;
	int freeze, laserTimer, numActive, initCount;
	XY vI, dI, laserSpawn; // vader index, death index
	Invader* lead;
public:
	Army(Game* g, DestroyedPixels* dpixels, int* wave);
	~Army() {}

	void update();
	void updateLasers();
	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);
	void drawInit(SDL_Surface* screen, SDL_Surface* spritesheet);
	void destroyLaser(int i);
	void laserCollided(XY* pos, int* i);
	void callExplosion(XY* pos);
	void setFreeze(int f) { freeze = f; frozen = true; }
	bool invaderDeath(XY* collPoint, int* t);
	bool laserOnLaser(XY* collPoint);
	bool* isFrozen() { return &frozen; }
	bool zigzagActive() { return (lasers.at(0) != nullptr); }
	int* numLeft() { return &numActive; }
	Invader* leadingInvader();
	XY* getLaserSpawn(bool targetplayer);

	vector<Laser*> getLasers() { return lasers; }
};

#endif
