#include "Army.hpp"
#include "Game.hpp"
#include "DestroyedPixels.hpp"
#include <algorithm>
#include <random>

auto rng = std::default_random_engine{};
vector<int> order = { 0, 1, 2 };

Army::Army(Game* g, DestroyedPixels* dpixels, int* wave)
{
	dp = dpixels; game = g;
	for (int i = 0; i < 3; i++) lasers.push_back(nullptr);
	vector<Invader*> row(11);
	vaders = vector<vector<Invader*>>(5, row);
	SDL_Rect rect; rect.x = 24; rect.w = 16; rect.h = 8;
	switch (*wave) {
	case 1:
		rect.y = 64;
		break;
	case 2:
		rect.y = 88;
		break;
	case 3:
		rect.y = 104;
		break;
	case 4: case 5: case 6:
		rect.y = 112;
		break;
	default:
		rect.y = 120;
		break;
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			if (i == 0) vaders.at(i).at(j) = new Invader(0, &rect);
			else if (i < 3) vaders.at(i).at(j) = new Invader(1, &rect);
			else vaders.at(i).at(j) = new Invader(2, &rect);
			rect.x += 16;
		}
		rect.y += 16; rect.x = 24;
	}
	dir = true; down = false; frozen = false; moving = false; queued = false; laserDestroyed = false;
	lead = this->leadingInvader();
	numActive = 55; fast = false; freeze = 0; 

	// set random laser buffer
	srand((unsigned)time(NULL));
	laserTimer = rand() % 70 + 25;
	initCount = 0;
	vI = XY(0, 0);
	dI = XY(0, 0);
	laserSpawn = XY(0, 0);
}

void Army::update()
{
	this->updateLasers();
	if (lead == NULL) { // no invaders left
		game->setWon(true); return; 
	}
	if (frozen) {
		freeze -= 1; if (freeze == 0) {
			vaders.at(dI.y).at(dI.x)->setActive(false); frozen = false;
			lead = this->leadingInvader();
			numActive--; if (numActive <= 1) fast = true;
			if (lead == NULL) return;
			if (*game->playerDied()) return; // lets a recently killed invader disappear
		}
	} else {
		if (*game->playerDied()) return;
		if (moving) {
			bool newMovement = false;
			while (true) {
				vI.x += (dir) ? -1 : 1;
				if (vI.x > 10 || vI.x < 0) { vI.x = (dir) ? 10 : 0; vI.y--; }
				if (vI.y < 0) { newMovement = true; break; }
				if (vaders.at(vI.y).at(vI.x)->isActive()) break;
			}
			if (!newMovement) {
				if (vaders.at(vI.y).at(vI.x)->move(&dir, &down, &fast)) {
					game->invaderReachedCannon();
				}
			} else moving = false;
		}
		if (!moving) {
			down = lead->isAtBorder(&dir); if (down) { 
				dir = !dir; lead = this->leadingInvader(); 
				game->startUFOTimer();
			}
			vI.y = 4; vI.x = (dir) ? 10 : 0;
			while (true) {
				if (vaders.at(vI.y).at(vI.x)->isActive()) break;
				vI.x += (dir) ? -1 : 1;
				if (vI.x > 10 || vI.x < 0) { vI.x = (dir) ? 10 : 0; vI.y--; }
			}
			if (vaders.at(vI.y).at(vI.x)->move(&dir, &down, &fast)) {
				game->invaderReachedCannon();
			}
			moving = true;
		}
	}
}

void Army::updateLasers()
{
	if (queued && laserDestroyed) { // there is now a spot for the queued laser
		std::shuffle(std::begin(order), std::end(order), rng);
		for (int i : order) {
			if (lasers.at(i) == nullptr) {
				if (i == 2) lasers.at(i) = new Laser(i, this->getLaserSpawn(true), NULL, this);
				else if (i == 0) if (*game->ufoActive()) continue;
				else lasers.at(i) = new Laser(i, this->getLaserSpawn(false), NULL, this);
				queued = false;
				laserTimer = rand() % 70 + 25;
				break;
			}
		}
		laserDestroyed = false;
	}
	if (!queued) {
		laserTimer--; if (laserTimer == 0) {
			bool thisFrame = false;
			std::shuffle(std::begin(order), std::end(order), rng);
			for (int i : order) {
				if (lasers.at(i) == nullptr) {
					if (i == 2) lasers.at(i) = new Laser(i, this->getLaserSpawn(true), NULL, this);
					else if (i == 0) if (*game->ufoActive()) continue;
					else lasers.at(i) = new Laser(i, this->getLaserSpawn(false), NULL, this);
					thisFrame = true;
					break;
				}
			}
			if (!thisFrame) queued = true; // couldn't spawn laser this frame
			else laserTimer = rand() % 70 + 25;
		}
	}
	for (int i = 0; i < 3; i++) if (lasers.at(i) != nullptr) lasers.at(i)->update();
}

void Army::destroyLaser(int i)
{
	if (lasers.at(i) == nullptr) return;
	lasers.at(i)->~Laser();
	lasers.at(i) = nullptr;
	laserDestroyed = true;
}

void Army::laserCollided(XY* pos, int* i)
{
	if (lasers.at(*i) != nullptr)
		lasers.at(*i)->setCollided(pos);
}

void Army::callExplosion(XY* pos)
{
	game->createExplosion(pos, false);
}

bool Army::invaderDeath(XY* collPoint, int* t)
{
	if (*t != 3) return false;
	bool coll = false;
	int i, j;
	for (i = 0; i < 5; i++) {
		for (j = 0; j < 11; j++) {
			if (!vaders.at(i).at(j)->isActive()) continue;
			if (vaders.at(i).at(j)->isWithin(collPoint)) { coll = true; goto gotcoll; }
		}
	}
gotcoll:
	if (coll) {
		printf("Invader %d %d shot down!\n", i, j);
		dI.x = j; dI.y = i; freeze += 16; frozen = true;
		vaders.at(i).at(j)->swapSprite(true);
		if (i == 0) game->addScore(30);
		else if (i < 3) game->addScore(20);
		else game->addScore(10);
	}
	return coll;
}

bool Army::laserOnLaser(XY* collPoint)
{
	for (int i = 0; i < 3; i++) {
		if (lasers.at(i) == nullptr) continue;
		if (lasers.at(i)->isWithin(collPoint)) {
			this->laserCollided(collPoint, &i);
			return true;
		}
	}
	return false;
}

Invader* Army::leadingInvader()
{
	int j = (dir) ? 10 : 0;
	while (true) {
		for (int i = 4; i > -1; i--) {
			if (vaders.at(i).at(j)->isActive()) return vaders.at(i).at(j);
		}
		j += (dir) ? -1 : 1;
		if (j > 10 || j < 0) break;
	}
	return NULL;
}

XY* Army::getLaserSpawn(bool targetplayer)
{
	// get a list of possible spawn points and use rand() to pick one
	// spawn point(column) is valid if there is an invader still in the column
	// y coordiate determined by invader closest to cannon

	vector<XY> points;
	for (int j = 0; j < 11; j++) {
		for (int i = 4; i > -1; i--) {
			if (vaders.at(i).at(j)->isActive()) {
				points.push_back(vaders.at(i).at(j)->laserSpawn());
				break;
			}
		}
	}
	if (!targetplayer) {
		laserSpawn = points.at(rand() % points.size());
		return &laserSpawn;
	}
	XY* cpos = game->getCannonPos(); XY closest;
	laserSpawn = points.at(0);
	for (int i = 0; i < points.size(); i++) {
		if (abs(points.at(i).x - cpos->x) < abs(laserSpawn.x - cpos->x))
			laserSpawn = points.at(i);
	}
	return &laserSpawn;
}

void Army::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 11; j++) {
			if (vaders.at(i).at(j)->draw(screen, spritesheet)) {
				// will return true if invaders are drawn over shields
				// aka we need to start destroying the shield pixels
				SDL_PixelFormat* dFormat = screen->format, * sFormat = spritesheet->format;
				SDL_Rect* vaderRect = vaders.at(i).at(j)->getDRect();
				int dStart = (screen->w * vaderRect->y) + vaderRect->x;
				int sStart = (spritesheet->w * vaderRect->y) + vaderRect->x;
				int pos, iSize = 8, jSize = 16; Uint8 r, g, b;
				for (int i = 0; i < iSize; i++) {
					for (int j = 0; j < jSize; j++) {
						// if screen pixel is already black, continue
						pos = dStart + (i * screen->w) + j;
						Uint32* dPixel = (Uint32*)screen->pixels + pos;
						SDL_GetRGB(*dPixel, dFormat, &r, &g, &b);
						if (r == 0) continue;
						// if not black then add pixel to dp
						dp->addPixel(pos);
					}
				}
			}
		}
	}
}

void Army::drawInit(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	int count = 0;
	while (count < initCount) {
		for (int i = 4; i > -1; i--) {
			for (int j = 0; j < 11; j++) {
				vaders.at(i).at(j)->draw(screen, spritesheet);
				count++; if (count == initCount) goto gtfo;
			}
		}
	}
gtfo:
	initCount++;
	if (initCount == 56) game->start();
}
