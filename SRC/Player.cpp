#include "Game.h"

// {{0.5f, 0.5f, 0.0f, 1.0f, 1.0f},
// {0.5f, -0.5f, 0.0f, 1.0f, 0.0f},
// {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f},
// {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f}};

void Player::init()
{
    anState.setAnimation(playerIdle);
    anState.vertices[0] = {0.5f, 0.5f, 0.0f, 1.0f, 1.0f};
    anState.vertices[1] = {0.5f, -0.5f, 0.0f, 1.0f, 0.0f};
    anState.vertices[2] = {-0.5f, -0.5f, 0.0f, 0.0f, 0.0f};
    anState.vertices[3] = {-0.5f, 0.5f, 0.0f, 0.0f, 1.0f};
};

void Player::update(float dt)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_D])
    {
        anState.setAnimation(playerRight);
        // !!! set players pos
    } else if (keys[SDL_SCANCODE_A]) {
        anState.setAnimation(playerLeft);
        // !!! set players pos
    } else {
        anState.setAnimation(playerIdle);
    }
}