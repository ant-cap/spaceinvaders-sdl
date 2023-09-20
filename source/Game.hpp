#ifndef game_hpp
#define game_hpp

#include "SDL.h"
#include "Utility.hpp"
#include "SurfaceManager.hpp"
#include "UI.hpp"
#include "Menu.hpp"
#include "Shield.hpp"
#include "Army.hpp"
#include "Cannon.hpp"
#include "UFO.hpp"
#include "Explosion.hpp"
#include "DestroyedPixels.hpp"
#include <vector>

class Game
{
private:
	SDL_Window* window;
	SurfaceManager* sman;
	UI* ui;
	Menu* menu;
	Army* army;
	Cannon* cannon;
	UFO* ufo;
	Explosion* laserExplosion;
	std::vector<Shield*> shields;
	std::vector<Explosion*> explosions;
	DestroyedPixels* dp;

	SDL_Rect seqSRect, seqDRect;

	int score, hiscore, lives, ufoTimer, count, wave;
	bool isRunning, playing, gameStart, playerDeath, gameOver, instaLose, won;
	
public:
	Game(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	~Game() {}

	void init(bool newgame);
	void reset(bool newgame);
	void handleEvents();
	void update();
	void render();
	void clean();

	void drawGame(SDL_Surface* drawsurface, SDL_Surface* spritesheet);
	void checkCollisions(SDL_Surface* drawsurface, SDL_Surface* spritesheet);
	void start() { gameStart = true; } 
	void createExplosion(XY* pos, bool type);
	void addScore(int add) { score += add; }
	void setPlaying(bool v) { playing = v; }
	void startUFOTimer();
	void timeForUFO();
	void setWon(bool w) { won = w; }
	void invaderReachedCannon();

	bool* running() { return &isRunning; }
	bool* getCredit() { return menu->getCredit(); }
	bool* isPlaying() { return &playing; }
	bool* ufoActive() { return ufo->isActive(); }
	bool* playerDied() { return cannon->isHit(); }
	bool shieldHit(XY* point);
	bool checkIfLost();

	int* getScore() { return &score; }
	int* getHiScore() { return &hiscore; }
	int* getLives() { return &lives; }
	int* getCannonShotsFired() { return cannon->getShotsFired(); }

	XY checkCollision(SDL_Surface* drawSurface, SDL_Surface* spritesheet, Laser* laser);
	XY* getCannonPos();
};

#endif
