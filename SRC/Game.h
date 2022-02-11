#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <initializer_list>
using namespace std;

enum AnimationEnum
{
    playerIdle = 0,
    playerLeft,
    playerRight,
    grass1,
    grass2,
    grass3,
    grass4,
    grass5,
    grassWithWater,
    tree1,
    tree2
};

struct AnimationState
{
    SDL_Rect rect;
    AnimationEnum animation;
    int currentFrame;
    float pointInRate;
    bool animationEnded = false;
    void inline setAnimation(AnimationEnum an)
    {
        animation = an;
        pointInRate = 0;
        currentFrame = 0;
    }
    void inline setAnimationSameFrame(AnimationEnum an)
    {
        animation = an;
    }
    bool inline didAnimationEnded()
    {
        return animationEnded;
    }
    void inline setRect(int x, int y, int w = 24, int h = 24)
    {
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
    }
};

class Object : public enable_shared_from_this<Object>
{
public:
    Object();
    ~Object();
    virtual void init();
    virtual void update(float dt);
    AnimationState anState;
};

struct Animation
{
    vector<SDL_Texture *> textures;
    float rate;
    bool noAnimation = false;
    bool oneCycle = false;
};

class Render
{
private:
    vector<Animation> animations;
    SDL_Renderer *renderer;
    SDL_Texture *loadTexture(const char *path);
    void addAnimation(float rate, initializer_list<SDL_Texture *> textures, bool noAnimation = false, bool oneCycle = false);

public:
    void init(SDL_Renderer *renderer);
    void loadAnimations();
    void update(float dt, vector<shared_ptr<Object>> *objects);
    void render(vector<shared_ptr<Object>> *objects);
};

struct ObjectList
{
public:
    vector<shared_ptr<Object>> objects;
    void add(shared_ptr<Object> obj);
    void remove(shared_ptr<Object> obj);
};

class Level
{
protected:
    Render render;

public:
    ObjectList _all;
    ObjectList _update;
    void virtual init(SDL_Renderer *renderer);
    void virtual update(float dt);
    void virtual inline draw()
    {
        render.render(&_all.objects);
    }
    void virtual remove(shared_ptr<Object> obj);
};
// to add object to level, call .add(...) to the lists the objects belong to

class LevelStart : public Level
{
public:
    ObjectList _foliage;
    ObjectList _ground;
    ObjectList _top;
    void init(SDL_Renderer *renderer) override;
    void inline draw() override;
};

class Player : public Object
{
    bool up = true;
    float counter = 0;

public:
    void init() override;
    void update(float dt) override;
};

class NoLogicObject : public Object
{
    int an;

public:
    NoLogicObject(AnimationEnum an, int x, int y, int h, int w);
};

class Game
{
    bool running;
    SDL_Event event;
    SDL_Window *window;
    SDL_Renderer *renderer;
    Uint32 lastUpdate;

public:
    Game();
    Level *level;
    void setLevel(Level *level);
    void init();
    void mainLoop();
};