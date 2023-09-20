#ifndef ui_hpp
#define ui_hpp

#include "SDL.h"
#include "Utility.hpp"
#include <map>
#include <string>

class Game;

class UI
{
private:
	std::map<char, XY> charMap;

	const char* welcomeText = "PLAySPACE  INVADERS=? MYSTERY=30 POINTS=20 POINTS=10 POINTS";
	const char* tableText = "*SCORE ADVANCE TABLE*";
	const char* scoreText = "SCORE<1> HI-SCORE SCORE<2>";
	const char* readyText = "PUSHONLY 1PLAYER  BUTTON";
	const char* playText = "PLAY PLAYER<1>";
	const char* creditText = "CREDIT 0";
	const char* gameoverText = "GAME OVER";
	Game* game;
	int flashTimer, ufoTimer, ufoPos;
	std::string ufoScore;
public:
	UI(Game* g);
	~UI() {}

	void startFlash() { flashTimer = 8; }
	void stopFlash() { flashTimer = -1; }
	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);
	void drawLives(SDL_Surface* screen, SDL_Surface* spritesheet);
	void drawScore(SDL_Surface* screen, SDL_Surface* spritesheet);
	void drawCredit(SDL_Surface* screen, SDL_Surface* spritesheet);
	void drawWelcome(SDL_Surface* screen, SDL_Surface* spritesheet, int* c);
	void drawReady(SDL_Surface* screen, SDL_Surface* spritesheet);
	void drawPlay(SDL_Surface* screen, SDL_Surface* spritesheet);
	void drawGameOver(SDL_Surface* screen, SDL_Surface* spritesheet, int* c);

	void setUFOScore(int shots, int xpos);
};

#endif
