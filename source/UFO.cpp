#include "UFO.hpp"
#include "Game.hpp"
#include "UI.hpp"

UFO::UFO(Game* g, UI* u)
{
	srcRect.x = 48; srcRect.y = 0; srcRect.w = 16; srcRect.h = 8;
	dstRect.x = 104; dstRect.y = 40; dstRect.w = 16; dstRect.h = 8;
	explosion = 0; initFired = 0; wait = 0; game = g; ui = u;
	destroy = false; active = false; direction = false;
}

void UFO::spawn()
{
	srcRect.x = 48; srcRect.y = 0; srcRect.w = 16; srcRect.h = 8;
	dstRect.w = 16; dstRect.h = 8; dstRect.y = 40;
	initFired = *game->getCannonShotsFired();
	if (initFired % 2 == 0) { direction = true; dstRect.x = 8; } 
	else { direction = false; dstRect.x = 200; }
	active = true; explosion = 0;
}

void UFO::update()
{
	if (explosion) {
		explosion++;
		if (explosion == 22) { 
			active = false; explosion = 0;
			int shotsForUFO = *game->getCannonShotsFired() - initFired;
			ui->setUFOScore(shotsForUFO, dstRect.x);
		}
	}
	else if (active) {
		wait++; if (wait == 3) {
			dstRect.x += (direction) ? 2 : -2;
			if (direction ? dstRect.x == 202 : dstRect.x == 6) active = false;
			wait = 0;
		}
	}
}

void UFO::explode()
{
	srcRect.x = 22; srcRect.y = 16; srcRect.w = 24; dstRect.w = 24; dstRect.x -= 4;
	explosion++;
}

void UFO::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	if (active)
	SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
}
