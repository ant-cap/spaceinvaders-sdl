#include "Game.hpp"

#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>

Game::Game(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0; if (fullscreen) flags = SDL_WINDOW_FULLSCREEN;

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		printf("Subsystems initialized\n");

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window) printf("Window created\n");
		else { printf("Window failed to create\n"); return; }

		sman = new SurfaceManager(this, window);

		isRunning = true;
	}
	else isRunning = false;

	// initialize variables

	this->init(true);
}

void Game::init(bool newgame)
{
	// set scores & other variables
	std::ifstream scorefile("score.txt"); std::string scorestring;
	if (scorefile.is_open()) {
		scorefile >> scorestring;
		hiscore = std::stoi(scorestring);
		std::cout << "score: " << score << " hiscore: " << hiscore << std::endl;
		scorefile.close();
	}
	else hiscore = 0;
	laserExplosion = nullptr;
	gameStart = false; playerDeath = false;
	gameOver = false; count = 0; won = false; instaLose = false;

	if (newgame) {
		wave = 1;
		score = 0;
		lives = 3;
		playing = false;
	}
	else {
		wave++;
		playing = true;
	}

	// game elements
	ui = new UI(this);
	menu = new Menu(this, ui);
	dp = new DestroyedPixels();
	army = new Army(this, dp, &wave);
	cannon = new Cannon(this);
	ufo = new UFO(this, ui);
	SDL_Rect shieldRect; shieldRect.x = 28; shieldRect.y = 192; shieldRect.w = 24; shieldRect.h = 16;
	for (int i = 0; i < 4; i++) { shields.push_back(new Shield(&shieldRect)); shieldRect.x += 45; }
}

void Game::reset(bool newgame)
{
	// will destroy all game objects and reinstantiate them
	ui->~UI(); menu->~Menu(); army->~Army(); cannon->~Cannon(); ufo->~UFO();
	for (int i = 0; i < 4; i++) shields.at(i)->~Shield(); shields.clear();
	dp->~DestroyedPixels();

	// calls init to complete process
	this->init(newgame);
}

void Game::handleEvents()
{
	const Uint8* kbstate = SDL_GetKeyboardState(NULL);
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
			else if (event.key.keysym.sym == SDLK_SPACE && !event.key.repeat) {
				if (!playing || !gameStart) {
					menu->incrementState();
				}
				else if (!*army->isFrozen() && laserExplosion == nullptr) 
					cannon->fire();
			}
			else if (event.key.keysym.sym == SDLK_w) { /*this->update(); this->render();*/ }
			break;
		case SDL_QUIT:
			isRunning = false;
			break;
		default:
			break;
	}

	bool l = false, r = false;
	if (kbstate[SDL_SCANCODE_LEFT]) l = true;
	if (kbstate[SDL_SCANCODE_RIGHT]) r = true;
	if (l && r) return; if (l) cannon->move(false); else if (r) cannon->move(true);
}

void Game::update()
{
	if (gameOver) {
		count++;
		if (count == 190) {
			menu->setState(7);
			menu->update();
		}
		return;
	}
	if (!playing || !gameStart) { menu->update(); return; }
	army->update();
	cannon->update();
	this->timeForUFO();
	ufo->update(); 
	if (*ufo->destroyMe()) { ufo->~UFO(); ufo = nullptr; }
	for (int i = 0; i < explosions.size(); i++) {
		if (*explosions.at(i)->destroyMe()) {
			explosions.at(i)->~Explosion();
			explosions.at(i) = nullptr;
		}
		else explosions.at(i)->update();
	}
	if (laserExplosion != nullptr) {
		if (*laserExplosion->destroyMe()) {
			laserExplosion->~Explosion();
			laserExplosion = nullptr;
		}
		else laserExplosion->update();
	}
	size_t initsize = explosions.size();
	explosions.erase(remove(explosions.begin(), explosions.end(), nullptr), explosions.end());

	if (won) {
		count++;
		if (count == 33) {
			menu->setState(8);
			menu->update();
		}
	}
}

void Game::render()
{
	sman->draw();
}

// function called by SurfaceManager
void Game::drawGame(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	/* DRAW ORDER :
		1. UI & Menu
		2. Shields
		3. DestroyedPixels
		4. Explosions (so "expired" explosions aren't drawn over active ones)
		5. Army
		6. Cannon
		7. UFO
		8. Lasers
	*/

	//if not playing yet, draw the menu
	if (!playing) {
		menu->draw(screen, spritesheet);
		return;
	}
	// let the army load
	if (!gameStart) {
		army->drawInit(screen, spritesheet);
		ui->draw(screen, spritesheet);
		for (Shield* s : shields) s->draw(screen, spritesheet);
		return;
	}
	ui->draw(screen, spritesheet);
	for (Shield* s : shields) s->draw(screen, spritesheet);
	dp->draw(screen);
	for (Explosion* e : explosions) e->draw(screen, spritesheet);
	if (laserExplosion != nullptr) laserExplosion->draw(screen, spritesheet);
	army->draw(screen, spritesheet);
	cannon->draw(screen, spritesheet);
	ufo->draw(screen, spritesheet);

	this->checkCollisions(screen, spritesheet);

	if (gameOver) {
		int c = int(count / 6) + 1;
		ui->drawGameOver(screen, spritesheet, &c);
		if (count >= 190) menu->draw(screen, spritesheet); // screen wipe
	}
	else if (won) {
		if (count >= 34) menu->draw(screen, spritesheet); // screen wipe
	}
}

void Game::checkCollisions(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	// check for laser collisions
	std::vector<Laser*> lasers(army->getLasers());
	lasers.push_back(cannon->getLaser());
	for (int i = 0; i < 4; i++) {
		if (lasers.at(i) == nullptr) continue;
		if (i < 3) if (lasers.at(i)->waiting()) {
			lasers.at(i)->draw(screen, spritesheet);
			continue;
		}
		XY coll = this->checkCollision(screen, spritesheet, lasers.at(i));
		if (coll.x == 0 && coll.y == 0) {				// no collision
			lasers.at(i)->draw(screen, spritesheet);
			continue;
		}
		if (coll.y == 36) {								// player laser missed
			printf("player cannon missed\n");
			cannon->laserCollided(&coll);
		}
		else if (coll.y >= 225) {						// enemy laser missed
			printf("enemy cannon missed\n");
			army->laserCollided(&coll, &i);
		}
		else if (coll.y <= 64) {						// ufo hit
			printf("Collided with ufo\n");
			ufo->explode();
			cannon->destroyLaser();
			lasers.at(i) = nullptr;
		}
		else if (coll.y >= 216) {						// player cannon hit
			printf("player was hit\n");
			cannon->initHit();
			army->laserCollided(&coll, &i);
		}
		else if (army->invaderDeath(&coll, &i)) {		// invader hit
			cannon->destroyLaser();
			lasers.at(i) = nullptr;
		}
		else if (this->shieldHit(&coll)) {				// shield hit
			printf("ShieldHit\n");
			if (i == 3) cannon->laserCollided(&coll);
			else army->laserCollided(&coll, &i);
		}
		else if (i == 3) {
			if (army->laserOnLaser(&coll)) {			// laser on laser collision
				cannon->destroyLaser();
				lasers.at(i) = nullptr;
			}
		}
		if (lasers.at(i) != nullptr)
			lasers.at(i)->draw(screen, spritesheet);
	}
}

XY Game::checkCollision(SDL_Surface* screen, SDL_Surface* spritesheet, Laser* laser)
{
	SDL_Rect* laserDRect = laser->getRect(true); SDL_Rect* laserSRect = laser->getRect(false);
	if (laserDRect->y <= 36) return XY(laserDRect->x, 36);
	if (laserDRect->y >= 229) return XY(laserDRect->x, laserDRect->y);
	int dStart = (screen->w * laserDRect->y) + laserDRect->x;
	int sStart = (spritesheet->w * laserSRect->y) + laserSRect->x;
	SDL_PixelFormat* dfmt = screen->format; SDL_PixelFormat* sfmt = spritesheet->format;
	uint8_t i, j; int pos; Uint32* pixel; Uint8 r, g, b; bool coll = false;
	SDL_LockSurface(screen); SDL_LockSurface(spritesheet);
	for (j = 0; j < 8; j++) {
		for (i = 0; i < 3; i++) {
			// check if source pixel is drawn
			pos = sStart + (j * spritesheet->w) + i;
			pixel = (Uint32*)spritesheet->pixels + pos;
			SDL_GetRGB(*pixel, sfmt, &r, &g, &b);
			if (r == 0) continue;
			// check if the pixel is already drawn on screen
			pos = dStart + (j * screen->w) + i;
			pixel = (Uint32*)screen->pixels + pos;
			SDL_GetRGB(*pixel, dfmt, &r, &g, &b);
			if (r == 0) continue;
			SDL_UnlockSurface(screen); SDL_UnlockSurface(spritesheet);
			return XY(laserDRect->x + i, laserDRect->y + j);
		}
	}
	SDL_UnlockSurface(screen); SDL_UnlockSurface(spritesheet);
	return XY(0, 0);
}

XY* Game::getCannonPos()
{
	return cannon->getFirePos();
}

void Game::createExplosion(XY* pos, bool type)
{
	if (type) {
		laserExplosion = new Explosion(pos, type, dp);
	}
	else {
		explosions.push_back(new Explosion(pos, type, dp));
	}
}

bool Game::shieldHit(XY* point)
{
	if (point->y < 192 || point->y > 207) return false;
	for (Shield* s : shields) if (s->isWithin(point)) return true;
	return false;
}

void Game::startUFOTimer()
{
	if (ufoTimer == -1)
	ufoTimer = 750;
}

void Game::timeForUFO()
{
	if (*army->numLeft() < 8) return;
	if (army->zigzagActive()) return;
	if (ufoTimer != -1) {
		// ufo timer active
		ufoTimer--; if (ufoTimer == 0) {
			ufo->spawn();
			ufoTimer = -1;
		}
	}
}

void Game::invaderReachedCannon()
{
	cannon->initHit();
	instaLose = true;
}

bool Game::checkIfLost()
{
	if (instaLose) lives = 0; else lives--;
	if (lives < 1) {
		gameOver = true;
		if (score > hiscore) {
			hiscore = score;
			std::ofstream scorefile("score.txt");
			scorefile << std::to_string(hiscore);
			scorefile.close();
			return true;
		}
	}
	return false;
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_Quit();
	printf("Game cleaned successfully\n");
}
