#include "Menu.hpp"
#include "Game.hpp"
#include "UI.hpp"

Menu::Menu(Game* g, UI* u)
{
	game = g; ui = u;

	wait = 0; count = -2; initwipe = 0; wipe = 0; wipefull = false; credit = false;
	state = 0; // init state
}

void Menu::update()
{
	switch (state) {
	case 1: // credit put in, present ready screen
		credit = true;
		wipe = 12; initwipe = 12;
		state--;
		break;
	case 3: // present play screen
		credit = false;
		wipe = 8; initwipe = 8; wipefull = true;
		state--; count = 180;
		break;
	case 5: // play the game!
		wipefull = false; wipe = 10; initwipe = 10;
		state--;
		break;
	case 6:
		game->setPlaying(true);
		ui->stopFlash();
		break;
	case 7: // game over case
		wipe = 12; initwipe = 12; wipefull = true;
		break;
	case 8: // win case
		wipe = 12; initwipe = 12; wipefull = true;
		break;
	}
}

void Menu::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	switch (state) {
	case 0: // init state, main menu
		ui->drawScore(screen, spritesheet);
		ui->drawCredit(screen, spritesheet);
		ui->drawWelcome(screen, spritesheet, &count);
		if (wait > 0) wait--;
		else {
			count++;
			if (count == 19) wait = 60;
			else wait = 6;
		}
		break;
	case 2: // coin in, wipe finished
		ui->drawScore(screen, spritesheet);
		ui->drawCredit(screen, spritesheet);
		ui->drawReady(screen, spritesheet);
		break;
	case 4: // play player<1>
		ui->drawScore(screen, spritesheet);
		ui->drawCredit(screen, spritesheet);
		ui->drawPlay(screen, spritesheet);
		count--; if (count == 0) state++;
		break;
	}
	if (wipe > 0) {
		this->wipeScreen(screen);
		wipe--; if (wipe == 0) {
			state+=2;
			if (initwipe == 8) ui->startFlash();
			else if (initwipe == 12) {
				if (state == 9) game->reset(true);
				else if (state == 10) game->reset(false);
			}
		}
	}
}

void Menu::wipeScreen(SDL_Surface* screen)
{
	SDL_Rect wipeRect; wipeRect.x = 0; wipeRect.y = (wipefull) ? 0 : 32;
	wipeRect.w = (224 / initwipe)*abs(wipe-initwipe); wipeRect.h = (wipefull) ? 256 : 208;
	SDL_FillRect(screen, &wipeRect, SDL_MapRGB(screen->format, 0, 0, 0));
}

bool Menu::forceWait()
{
	if (wipe == 0) return false;
	if (state == 4) return false;
	return true;
}
