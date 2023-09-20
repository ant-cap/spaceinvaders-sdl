#include "Shield.hpp"

Shield::Shield(SDL_Rect* dstR)
{
	srcRect.x = 64; srcRect.y = 0; srcRect.w = 24; srcRect.h = 16;
	dstRect = *dstR;
}

void Shield::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
}

bool Shield::isWithin(XY* point)
{
	if ((dstRect.x <= point->x && dstRect.x + 24 >= point->x) &&
		(dstRect.y <= point->y && dstRect.y + 16 >= point->y)) return true;
	return false;
}
