#include "Game.h"

void Player::init()
{
    anState.setAnimation(0);
    anState.rect = {200, 200, 60, 81};
}

void Player::update(float dt)
{
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_D])
    {
        anState.setAnimation(2);
        anState.rect.x += 3;
    } else if (keys[SDL_SCANCODE_A]) {
        anState.setAnimation(1);
        anState.rect.x -= 3;
    } else {
        anState.setAnimation(0);
    }
}