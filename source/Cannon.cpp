#include "Cannon.hpp"
#include "Game.hpp"

Cannon::Cannon(Game* g)
{
	srcRect.x = 48; srcRect.y = 8; srcRect.w = 16; srcRect.h = 8;
	dstRect.x = 8; dstRect.y = 216; dstRect.w = 16; dstRect.h = 8;
	laser = nullptr; freshlaser = false; firePos = XY(0, 0);
	game = g; shotsFired = 0; hit = false; sprite = false; count = 0;
}

void Cannon::update()
{
	if (laser != nullptr) {
		if (!freshlaser) laser->update();
		else freshlaser = false;
	}
	if (hit) {
		count++; if (count == 188) {
			hit = false;
			sprite = false;
			dstRect.x = 8; dstRect.y = 216;
			srcRect.x = 48; srcRect.y = 8;
			count = 0;
		}
		else if (count < 60) {
			if (count % 5 == 0) {
				srcRect.x += (sprite) ? -16 : 16;
				sprite = !sprite;
			}
		}
		else if (count == 60) {
			game->checkIfLost();
		}
	}
}

void Cannon::move(bool dir)
{
	if (hit) return;
	if ((dir && dstRect.x >= 200) || (!dir && dstRect.x <= 8)) return;
	dstRect.x += (dir) ? 1 : -1;
}

void Cannon::fire()
{
	if (hit) return;
	if (laser != nullptr) return;
	laser = new Laser(3, this->getFirePos(), this, NULL);
	freshlaser = true;
	shotsFired++;
}

void Cannon::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	if (count < 60)
	SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
}

void Cannon::initHit()
{
	hit = true;
	srcRect.x = 46; srcRect.y = 16;
}

void Cannon::laserCollided(XY* pos)
{
	if (laser != nullptr) 
		laser->setCollided(pos);
}

void Cannon::callExplosion(XY* pos)
{
	game->createExplosion(pos, true);
}

void Cannon::destroyLaser()
{
	if (laser == nullptr) return;
	laser->~Laser(); laser = nullptr;
}

XY* Cannon::getFirePos()
{
	firePos = XY(dstRect.x + 7, dstRect.y - 4);
	return &firePos;
}
