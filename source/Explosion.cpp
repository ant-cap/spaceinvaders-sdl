#include "Explosion.hpp"
#include "DestroyedPixels.hpp"

Explosion::Explosion(XY* pos, bool t, DestroyedPixels* destroyedPixels)
{
	dp = destroyedPixels; type = t; dstRect.x = pos->x; dstRect.y = pos->y;
	timer = 0;
	if (type) {
		// this is a player explosion
		srcRect.x = 36; srcRect.y = 24; srcRect.w = 8; srcRect.h = 8;
		dstRect.x -= 4; dstRect.y -= 2; dstRect.w = 8; dstRect.h = 8;
		timer = 14;
	} else {
		// this is an invader explosion
		srcRect.x = 16; srcRect.y = 16; srcRect.w = 6; srcRect.h = 8;
		dstRect.x -= 3; dstRect.y -= 4; dstRect.w = 6; dstRect.h = 8;
		if (pos->y >= 229) dstRect.y = 232;
		timer = 8;
	}
	destroy = false;
}

void Explosion::update()
{
	if (timer > 0) { timer--; if (timer == 0) destroy = true; }
}

void Explosion::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	if (destroy) {
		SDL_PixelFormat* dFormat = screen->format, * sFormat = spritesheet->format;
		int dStart = (screen->w * dstRect.y) + dstRect.x;
		int sStart = (spritesheet->w * srcRect.y) + srcRect.x;
		int pos, iSize = 8, jSize = (type) ? 8 : 6; Uint8 r, g, b;
		for (int i = 0; i < iSize; i++) {
			for (int j = 0; j < jSize; j++) {
				// if screen pixel is already black, continue
				pos = dStart + (i * screen->w) + j;
				Uint32* dPixel = (Uint32*)screen->pixels + pos;
				SDL_GetRGB(*dPixel, dFormat, &r, &g, &b);
				if (r == 0) continue;
				// check if source pixel is drawn
				Uint32* sPixel = (Uint32*)spritesheet->pixels + (sStart + (i * spritesheet->w) + j);
				SDL_GetRGB(*sPixel, sFormat, &r, &g, &b);
				if (r == 0) continue;
				// if drawn, add pixel to DestroyedPixels class
				dp->addPixel(pos);
			}
		}
	}
	SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
}
