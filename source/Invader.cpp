#include "Invader.hpp"

Invader::Invader(uint8_t t, SDL_Rect* d)
{
	type = t;
	dstRect = *d;
	srcRect.x = 0; srcRect.y = 0; srcRect.w = 16; srcRect.h = 8;
	if (type == 1) srcRect.x = 16;
	else if (type == 2) srcRect.x = 32;
	active = true;
}

bool Invader::draw(SDL_Surface* surface, SDL_Surface* spritesheet)
{
	if (active) SDL_BlitSurface(spritesheet, &srcRect, surface, &dstRect);
	if (dstRect.y == 192 || dstRect.y == 200) {
		// start clearing the shield pixels (add them to destroyedPixels)
		if (dstRect.x <= 12 || dstRect.x >= 187) return false;
		return true;
	}
	return false;
}

bool Invader::move(bool* direction, bool* down, bool* fast)
{
	int8_t speed = 2;
	dstRect.x += (*direction) ? speed + (int)(*fast) : -speed;
	if (*down) dstRect.y += 8;
	this->swapSprite(false);
	if (dstRect.y == 216) return true;
	return false;
}

void Invader::swapSprite(bool dead)
{
	if (dead) { srcRect.x = 0; srcRect.y = 16; }
	else srcRect.y += (srcRect.y == 0) ? 8 : -8;
}

bool Invader::isWithin(XY* point)
{
	if ((dstRect.x <= point->x && dstRect.x + 16 >= point->x) &&
		(dstRect.y <= point->y && dstRect.y + 8 >= point->y)) return true;
	return false;
}
