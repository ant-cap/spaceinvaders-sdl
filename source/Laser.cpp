#include "Laser.hpp"
#include "Cannon.hpp"
#include "Army.hpp"

Laser::Laser(int t, XY* pos, Cannon* c, Army* a)
{
	dstRect.w = 3; dstRect.h = 3; dstRect.x = pos->x; dstRect.y = pos->y; frame = 0;
	type = t; if (type == 3) /*PLAYER*/ srcRect.x = 33;
	else {
		if (type == 0)		 /*ZIGZAG*/ frames = { 3, 6, 9, 0 };
		else if (type == 1)  /*SLIDER*/ frames = { 15, 18, 21, 12 };
		else if (type == 2)  /*PULSER*/ frames = { 27, 24, 30, 24 };
		srcRect.x = frames.at(frame);
	}
	srcRect.y = 24; srcRect.w = 3; srcRect.h = 8;
	sender = (type == 3) ? true : false; collided = false;
	army = a; cannon = c; wait = 0;
	colPos = nullptr;
}

void Laser::update()
{
	if (type == 3) {  // PLAYER
		if (collided) {
			cannon->callExplosion(colPos);
			cannon->destroyLaser();
			return;
		}
		dstRect.y -= 4;
	}
	else {			  // INVADER
		wait++; if (wait == 3) {
			if (collided) { 
				army->callExplosion(colPos);
				army->destroyLaser(type);
				return; 
			}
			frame++; if (frame > 3) frame = 0;
			srcRect.x = frames.at(frame);
			dstRect.y += 4;
			wait = 0;
		}
	}
}

bool Laser::isWithin(XY* pos)
{
	printf("Pos: [%d %d]\nLas: [%d %d]\n", pos->x, pos->y, dstRect.x, dstRect.y);
	if ((pos->x >= dstRect.x) && (pos->x < dstRect.x + 3) &&
	   ((pos->y >= dstRect.y) && (pos->y < dstRect.y + 8))) {
		printf("omg true!\n");
		return true;
	}
	return false;
}

void Laser::setCollided(XY* pos)
{
	collided = true;
	colPos = new XY(pos->x, pos->y);
}

void Laser::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
}
