#include "UI.hpp"
#include "Game.hpp"

void createCharMap(std::map<char, XY>& cmap)
{
	uint8_t sx = 0; uint8_t sy = 32; // source x and source y for spritesheet
	cmap['A'] = XY(sx, sy); sx += 8;
	cmap['B'] = XY(sx, sy); sx += 8;
	cmap['C'] = XY(sx, sy); sx += 8;
	cmap['D'] = XY(sx, sy); sx += 8;
	cmap['E'] = XY(sx, sy); sx += 8;
	cmap['F'] = XY(sx, sy); sx += 8;
	cmap['G'] = XY(sx, sy); sx += 8;
	cmap['H'] = XY(sx, sy); sx += 8;
	cmap['I'] = XY(sx, sy); sx += 8;
	cmap['J'] = XY(sx, sy); sx += 8;
	cmap[' '] = XY(sx, sy); sx = 0; sy += 8;
	cmap['K'] = XY(sx, sy); sx += 8;
	cmap['L'] = XY(sx, sy); sx += 8;
	cmap['M'] = XY(sx, sy); sx += 8;
	cmap['N'] = XY(sx, sy); sx += 8;
	cmap['O'] = XY(sx, sy); sx += 8;
	cmap['P'] = XY(sx, sy); sx += 8;
	cmap['Q'] = XY(sx, sy); sx += 8;
	cmap['R'] = XY(sx, sy); sx += 8;
	cmap['S'] = XY(sx, sy); sx += 8;
	cmap['T'] = XY(sx, sy); sx += 8; 
	cmap['y'] = XY(sx, sy); sx = 0; sy += 8;
	cmap['U'] = XY(sx, sy); sx += 8;
	cmap['V'] = XY(sx, sy); sx += 8;
	cmap['W'] = XY(sx, sy); sx += 8;
	cmap['X'] = XY(sx, sy); sx += 8;
	cmap['Y'] = XY(sx, sy); sx += 8;
	cmap['Z'] = XY(sx, sy); sx += 8;
	cmap['0'] = XY(sx, sy); sx += 8;
	cmap['1'] = XY(sx, sy); sx += 8;
	cmap['2'] = XY(sx, sy); sx += 8;
	cmap['3'] = XY(sx, sy); sx += 8;
	cmap['-'] = XY(sx, sy); sx = 0; sy += 8;
	cmap['4'] = XY(sx, sy); sx += 8;
	cmap['5'] = XY(sx, sy); sx += 8;
	cmap['6'] = XY(sx, sy); sx += 8;
	cmap['7'] = XY(sx, sy); sx += 8;
	cmap['8'] = XY(sx, sy); sx += 8;
	cmap['9'] = XY(sx, sy); sx += 8;
	cmap['<'] = XY(sx, sy); sx += 8;
	cmap['>'] = XY(sx, sy); sx += 8;
	cmap['='] = XY(sx, sy); sx += 8;
	cmap['*'] = XY(sx, sy); sx += 8;
	cmap['?'] = XY(sx, sy);

}

UI::UI(Game* g)
{
	game = g;
	createCharMap(charMap);
	//printf("MAKE SURE THIS WORKS: %d %d\n", charMap['<'].x, charMap['<'].y);
	flashTimer = -1;
	ufoScore = ""; ufoTimer = 0; ufoPos = 0;
}

void UI::draw(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	this->drawScore(screen, spritesheet);
	this->drawCredit(screen, spritesheet);
	this->drawLives(screen, spritesheet);

	// draw tha line
	SDL_Rect srcRect, dstRect;
	srcRect.x = 7; srcRect.y = 0; srcRect.w = 1; srcRect.h = 1;
	dstRect.x = 0; dstRect.y = 239; dstRect.w = 224; dstRect.h = 1;
	SDL_BlitScaled(spritesheet, &srcRect, screen, &dstRect);
	
	// if ufoTimer, draw the ufo score
	if (ufoTimer != 0) {
		srcRect.w = 8; srcRect.h = 8; dstRect.w = 8; dstRect.h = 8;
		dstRect.x = ufoPos; dstRect.y = 40;
		for (int i = int(ufoScore.size()) - 1; i > -1; i--) {
			srcRect.x = charMap[ufoScore[i]].x;
			srcRect.y = charMap[ufoScore[i]].y;
			SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
			dstRect.x -= 8;
		}
		ufoTimer--;
	}
}

void UI::drawLives(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	SDL_Rect dstRect; dstRect.x = 8; dstRect.y = 240; dstRect.w = 8; dstRect.h = 8;
	SDL_Rect srcRect; srcRect.w = 8; srcRect.h = 8;
	srcRect.x = charMap[std::to_string(*game->getLives())[0]].x;
	srcRect.y = charMap[std::to_string(*game->getLives())[0]].y;
	SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
	dstRect.x += 16; dstRect.w = 16;
	srcRect.x = 48; srcRect.y = 8; srcRect.w = 16;
	for (int i = 1; i < *game->getLives(); i++) {
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 16;
	}
}

void UI::drawScore(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	SDL_Rect dstRect; dstRect.x = 8; dstRect.y = 8; dstRect.w = 8; dstRect.h = 8;
	SDL_Rect srcRect; srcRect.w = 8; srcRect.h = 8;
	// score text
	for (int i = 0; i < 26; i++) {
		srcRect.x = charMap[scoreText[i]].x;
		srcRect.y = charMap[scoreText[i]].y;
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 8;
	}
	// scores
	std::string scoreString = std::to_string(*game->getScore());
	std::string hiscoreString = std::to_string(*game->getHiScore());
	while (scoreString.length() < 4) scoreString.insert(0, "0");
	while (hiscoreString.length() < 4) hiscoreString.insert(0, "0");
	if (scoreString.length() > 4) scoreString = scoreString.substr(scoreString.length() - 4, 4);
	if (hiscoreString.length() > 4) hiscoreString = hiscoreString.substr(hiscoreString.length() - 4, 4);

	bool showScore = true;
	if (flashTimer != -1) {
		if (flashTimer == 8) flashTimer = 7;
		if (flashTimer < 4) showScore = false;
		flashTimer++; if (flashTimer == 8) flashTimer = 0;
	}

	dstRect.x = 8 * 3; dstRect.y = 8 * 3;
	for (int i = 0; i < 12; i++) {
		if (i < 4) {
			if (showScore) {
				srcRect.x = charMap[scoreString.at(i)].x;
				srcRect.y = charMap[scoreString.at(i)].y;
			}
			else {
				srcRect.x = charMap[' '].x;
				srcRect.y = charMap[' '].y;
			}
		}
		else if (i < 8) {
			srcRect.x = charMap[' '].x;
			srcRect.y = charMap[' '].y;
		}
		else {
			srcRect.x = charMap[hiscoreString.at(i - 8)].x;
			srcRect.y = charMap[hiscoreString.at(i - 8)].y;
		}
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 8;
	}
}

void UI::drawCredit(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	SDL_Rect dstRect; dstRect.x = 136; dstRect.y = 240; dstRect.w = 8; dstRect.h = 8;
	SDL_Rect srcRect; srcRect.w = 8; srcRect.h = 8;
	for (int i = 0; i < 8; i++) {
		srcRect.x = charMap[creditText[i]].x;
		srcRect.y = charMap[creditText[i]].y;
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 8;
	}
	srcRect.x = (*game->getCredit()) ? charMap['1'].x : charMap['0'].x;
	srcRect.y = (*game->getCredit()) ? charMap['1'].y : charMap['0'].y;
	SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
}

void UI::drawWelcome(SDL_Surface* screen, SDL_Surface* spritesheet, int* c)
{
	int count = *c; if (count > 59) count = 59; if (count < 1) return;
	SDL_Rect dstRect; dstRect.x = 96; dstRect.y = 64; dstRect.w = 8; dstRect.h = 8;
	SDL_Rect srcRect; srcRect.w = 8; srcRect.h = 8;
	for (int i = 0; i < count; i++) {
		if (i == 4) {
			dstRect.x = 56; dstRect.y += 24;
		}
		if (i == 19) { // draw table
			dstRect.x = 32; dstRect.y += 32;
			for (int j = 0; j < 21; j++) {
				srcRect.x = charMap[tableText[j]].x;
				srcRect.y = charMap[tableText[j]].y;
				SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
				dstRect.x += 8;
			}
			// creatures
			srcRect.w = 16; dstRect.w = 16; dstRect.x = 64;
			/* ufo */ srcRect.x = 48; srcRect.y = 0; dstRect.y += 16;
			SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
			/*inv 1*/ srcRect.x = 0; dstRect.y += 16;
			SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
			/*inv 2*/ srcRect.x += 16; dstRect.y += 16;
			SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
			/*inv 3*/ srcRect.x += 16; dstRect.y += 16;
			SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
			dstRect.y -= 64; dstRect.w = 8; srcRect.w = 8;
		}
		if (i == 19 || i == 29 || i == 39 || i == 49) { 
			dstRect.x = 80; dstRect.y += 16; }
		srcRect.x = charMap[welcomeText[i]].x;
		srcRect.y = charMap[welcomeText[i]].y;
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 8;
	}

}

void UI::drawReady(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	SDL_Rect dstRect; dstRect.x = 96; dstRect.y = 88; dstRect.w = 8; dstRect.h = 8;
	SDL_Rect srcRect; srcRect.w = 8; srcRect.h = 8;
	for (int i = 0; i < 24; i++) {
		if (i == 4) { dstRect.x = 32; dstRect.y += 24; }
		srcRect.x = charMap[readyText[i]].x;
		srcRect.y = charMap[readyText[i]].y;
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 8;
	}
}

void UI::drawPlay(SDL_Surface* screen, SDL_Surface* spritesheet)
{
	SDL_Rect dstRect; dstRect.x = 56; dstRect.y = 112; dstRect.w = 8; dstRect.h = 8;
	SDL_Rect srcRect; srcRect.w = 8; srcRect.h = 8;
	for (int i = 0; i < 14; i++) {
		srcRect.x = charMap[playText[i]].x;
		srcRect.y = charMap[playText[i]].y;
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 8;
	}
}

void UI::drawGameOver(SDL_Surface* screen, SDL_Surface* spritesheet, int* c)
{
	int count = *c; if (count > 9) count = 9;
	SDL_Rect dstRect; dstRect.x = 72; dstRect.y = 56; dstRect.w = 8; dstRect.h = 8;
	SDL_Rect srcRect; srcRect.w = 8; srcRect.h = 8;
	for (int i = 0; i < count; i++) {
		srcRect.x = charMap[gameoverText[i]].x;
		srcRect.y = charMap[gameoverText[i]].y;
		SDL_BlitSurface(spritesheet, &srcRect, screen, &dstRect);
		dstRect.x += 8;
	}
}

void UI::setUFOScore(int shots, int xpos) {
	int ufoScore_int;
	if (shots == 1) ufoScore_int = 300;
	else if (shots == 2) ufoScore_int = 150;
	else if (shots < 5) ufoScore_int = 100;
	else ufoScore_int = 50;

	ufoScore = std::to_string(ufoScore_int);
	ufoTimer = 73;
	ufoPos = xpos + 16; // offset for text
	game->addScore(ufoScore_int);
}
