#include "SDL.h"
#include "Game.hpp"

Game* game = nullptr;

int main(int argc, char* argv[])
{
	const int FPS = 60;
	const float frameDelay = 1000.0f / (float)FPS;

	game = new Game("SpaceInvaders-sdl by antcap [224x256]", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 224*3, 256*3, false);

	while (*game->running()) {
		Uint64 start = SDL_GetPerformanceCounter();
		// handleEvents, update, render in that order
		game->handleEvents();
		game->update();
		game->render();
		Uint64 end = SDL_GetPerformanceCounter();
		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;

		// cap to FPS
		SDL_Delay((Uint32)floor(frameDelay - elapsedMS));
	}

	game->clean();
	
	return 0;
}
