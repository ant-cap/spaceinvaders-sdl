#ifndef menu_hpp
#define menu_hpp

#include "SDL.h"

class Game;
class UI;

class Menu
{
private:
	int state, count, wait, wipe, initwipe;
	bool wipefull, credit;

	Game* game;
	UI* ui;
public:
	Menu(Game* g, UI* u);
	~Menu() {}

	void update();
	void draw(SDL_Surface* screen, SDL_Surface* spritesheet);
	void wipeScreen(SDL_Surface* screen);

	void incrementState() { state++; }
	void decrementState() { state--; }
	void setState(int s) { state = s; }
	bool forceWait();
	bool* getCredit() { return &credit; }
};

#endif
