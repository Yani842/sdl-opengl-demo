#include "Game.h"
#include <cmath>
using namespace std;

Game::Game() {}

void Game::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	SDL_Event event;
	window = SDL_CreateWindow("Just a typical game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	lastUpdate = SDL_GetTicks();
	running = true;
    level = new LevelStart();
    level->init(renderer);
}

void Game::setLevel(Level* level)
{
    level = level;
}

void Game::mainLoop()
{
    while (running)
    {
        Uint64 start = SDL_GetTicks();
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        Uint32 current = SDL_GetTicks();
        float dt = (current - lastUpdate) / 1000.0f;
        level->update(dt);
        char buffer[64];
        int ret = snprintf(buffer, sizeof buffer, "%f", dt);
        SDL_SetWindowTitle(window, buffer);

        SDL_RenderClear(renderer);
        level->draw();
        SDL_RenderPresent(renderer);

        lastUpdate = current;
        Uint64 end = SDL_GetTicks();
        auto delay = end - start;
        if (delay < 16.666f)
        {
            SDL_Delay(16.666f - delay);
        }
    }
}