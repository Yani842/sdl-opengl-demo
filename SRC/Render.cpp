#include "Game.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Render::Render()
{
}

unsigned int Render::loadTexture(const char *path)
{
    string strPath = "../Data/Images/";
    strPath += path;
    unsigned int tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(strPath.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture\n";
    }
    stbi_image_free(data);
    return tex;
}

void Render::addAnimation(float rate, initializer_list<unsigned int> textures, bool noAnimation, bool oneCycle)
{
    Animation an;
    an.textures = textures;
    an.rate = rate;
    an.noAnimation = noAnimation;
    an.oneCycle = oneCycle;
    animations.push_back(an);
}

void Render::init()
{
    shader.init("../Data/Shaders/shader.vs", "../Data/Shaders/shader.fs");
    shader.use();
    shader.setInt("tex1", 0);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    glGenBuffers(255, VBOs);

    glBindVertexArray(VAO);

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3};

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render::loadAnimations()
{
    animations.clear();
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

void Render::bind(vector<shared_ptr<Object>> *objects, unsigned char layer)
{
    layers[layer] = objects;
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[layer]);
    vertex vertices[objects->size() * 4];
    // adding all object's vertices to one array
    for (int i = 0; i < objects->size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            vertices[i * 4 + j] = objects->at(i)->anState.vertices[j];
        }
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // pos attr
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texcoord attr
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Render::unbind(unsigned char layer)
{
    layers[layer] = NULL;
    glDeleteBuffers(1, &VBOs[layer]);
}

void Render::update(float dt, vector<shared_ptr<Object>> *objects)
{
    for (auto obj : *objects)
    {
        if (obj == NULL)
        {
            continue;
        }
        auto *st = &obj->anState;
        auto *an = &animations.at(st->animation);
        if (st->didAnimationEnded() || an->noAnimation)
        {
            continue;
        }
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

void Render::render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto layer : layers)
    {
        if (layer == 0)
        {
            continue;
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, loadTexture("ghost/idle-1.png")); // animations.at(0).textures.at(0));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}