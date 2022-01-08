#include "Game.h"

Render::Render()
{

}

SDL_Texture* Render::loadTexture(const char* path)
{
    string strPath = "Data/Images/";
    strPath += path;
    SDL_Surface *tmp = IMG_Load(strPath.c_str());
    SDL_Texture *tex = SDL_CreateTextureFromSurface(renderer, tmp);
    SDL_FreeSurface(tmp);
    return tex;
}

void Render::addAnimation(float rate, initializer_list<SDL_Texture*> textures, bool noAnimation, bool oneCycle)
{
    Animation an;
    an.textures = textures;
    an.rate = rate;
    an.noAnimation = noAnimation;
    an.oneCycle = oneCycle;
    animations.push_back(an);
}

void Render::init(SDL_Renderer* ren)
{
    renderer = ren;
    addAnimation(0, {loadTexture("ghost/idle-1.png")}, true);
    addAnimation(0, {loadTexture("ghost/left-1.png")}, true);
    addAnimation(0, {loadTexture("ghost/right-1.png")}, true);
    addAnimation(0, {loadTexture("ground/grass-1.png")}, true);
    addAnimation(0, {loadTexture("ground/grass-2.png")}, true);
    addAnimation(0, {loadTexture("ground/grass-3.png")}, true);
    addAnimation(0, {loadTexture("ground/grass-4.png")}, true);
    addAnimation(0, {loadTexture("ground/grass-5.png")}, true);
    addAnimation(0, {loadTexture("ground/grass+water.png")}, true);
    addAnimation(0, {loadTexture("foliage/tree-1.png")}, true);
    addAnimation(0, {loadTexture("foliage/tree-2.png")}, true);
}

void Render::update(float dt, vector<shared_ptr<Object>>* objects)
{
    for (auto obj : *objects)
    {
        if (obj == NULL) {continue;}
        auto* st = &obj->anState;
        auto* an = &animations.at(st->animation);
        if (st->didAnimationEnded() || an->noAnimation) {continue;}
        st->pointInRate += dt;
        if (st->pointInRate > an->rate)
        {
            st->pointInRate = 0;
            st->currentFrame++;
            if (st->currentFrame >= an->textures.size())
            {
                st->currentFrame = 0;
                if (an->oneCycle)
                {
                    st->animationEnded = true;
                }
            }
        }
    }
}

void Render::render(vector<shared_ptr<Object>>* objects)
{
    for (auto obj : *objects)
    {
        if (obj == NULL || obj->anState.didAnimationEnded()) {continue;}
        auto st = obj->anState;
        SDL_RenderCopy(renderer, animations.at(st.animation).textures.at(st.currentFrame), NULL, &st.rect);
    }
}