#pragma once
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>
#include <initializer_list>
#include "Shader.h"
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

struct vertex
{
    struct pos
    {
        float x;
        float y;
        float z;
    } pos;
    struct texCoords
    {
        float x;
        float y;
    } tex;
};
struct AnimationState
{
private:

public:
    vertex vertices[4];

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
    vector<unsigned int> textures;
    float rate;
    bool noAnimation = false;
    bool oneCycle = false;
};

class Render
{
private:
    unsigned int VAO, EBO;
    unsigned int VBOs[255] = {0};
    Shader shader;
    vector<shared_ptr<Object>> *layers[255];
    vector<Animation> animations;
    unsigned int loadTexture(const char *path);
    void addAnimation(float rate, initializer_list<unsigned int> textures, bool noAnimation = false, bool oneCycle = false);

public:
    Render();
    void init();
    void loadAnimations();
    void bind(vector<shared_ptr<Object>> *objects, unsigned char layer);
    void unbind(unsigned char layer);
    void update(float dt, vector<shared_ptr<Object>> *objects);
    void render();
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
    ObjectList _all;
    ObjectList _update;
    ObjectList _foliage;
    ObjectList _ground;
    ObjectList _top;

    bool running = true;
    SDL_Window *window;

    void update(float dt);

public:
    Level();
    void init(string title, int width, int height);
    void run();
    void remove(shared_ptr<Object> obj);
};
// to add object to level, call .add(...) to the lists the objects belong to

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
