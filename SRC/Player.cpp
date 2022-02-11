#include "Game.h"

void Player::init()
{
    anState.setAnimation(playerIdle);
    anState.rect = {200, 200, 60, 81};
}

void Player::update(float dt)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_D])
    {
        anState.setAnimation(playerRight);
        anState.rect.x += 3;
    } else if (keys[SDL_SCANCODE_A]) {
        anState.setAnimation(playerLeft);
        anState.rect.x -= 3;
    } else {
        anState.setAnimation(playerIdle);
    }
}