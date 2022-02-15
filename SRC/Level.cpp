#include "Game.h"

void ObjectList::add(shared_ptr<Object> obj)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if(objects.at(i) == NULL)
        {
            objects.at(i) = obj;
            return;
        }
    }
    objects.push_back(obj);
}

void ObjectList::remove(shared_ptr<Object> obj)
{
    objects.at(find(objects.begin(), objects.end(), obj) - objects.end()) = NULL;
}

void Level::remove(shared_ptr<Object> obj)
{
    _all.remove(obj);
    _update.remove(obj);
    _foliage.remove(obj);
    _ground.remove(obj);
    _top.remove(obj);
    // if there will be more lists add them here
}

Level::Level() {}

void Level::init(string title, int width, int height) 
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cout << "window didnt initialized!\n";
        running = false;
    }
    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (glContext == nullptr)
    {
        std::cout << "gl context didnt initialized!\n";
        running = false;
    }
    GLenum error = glewInit();
    if (error != GLEW_OK)
    {
        std::cout << "glew didnt initialized!\n";
        running = false;
    }

    render.init();
    render.loadAnimations();
    
    shared_ptr<Object> pl (new Player());
    _all.add(pl);
    _update.add(pl);
    _top.add(pl);
    pl->init();

    // shared_ptr<Object> gr1 (new NoLogicObject(grass1, 0, 200, 60, 60));
    // _all.add(gr1);
    // _ground.add(gr1);
    // shared_ptr<Object> gr2 (new NoLogicObject(grass2, 60, 200, 60, 60));
    // _all.add(gr2);
    // _ground.add(gr2);
    // shared_ptr<Object> gr3 (new NoLogicObject(grass3, 120, 200, 60, 60));
    // _all.add(gr3);
    // _ground.add(gr3);
    // shared_ptr<Object> gr4 (new NoLogicObject(grass4, 180, 200, 60, 60));
    // _all.add(gr4);
    // _ground.add(gr4);
    // shared_ptr<Object> gr5 (new NoLogicObject(grass5, 240, 200, 60, 60));
    // _all.add(gr5);
    // _ground.add(gr5);
    // shared_ptr<Object> gr6 (new NoLogicObject(grassWithWater, 300, 200, 60, 60));
    // _all.add(gr6);
    // _ground.add(gr6);
    // shared_ptr<Object> tr1 (new NoLogicObject(tree1, 100, 200, 45*4, 75*4));
    // _all.add(tr1);
    // _foliage.add(tr1);
    // shared_ptr<Object> tr2 (new NoLogicObject(tree2, 300, 200, 45*4, 75*4));
    // _all.add(tr2);
    // _foliage.add(tr2);

    render.bind(&_top.objects, 1);
}

void Level::run()
{
    Uint32 lastUpdate = SDL_GetTicks();
    while (running)
    {
        Uint64 start = SDL_GetTicks();
        
        SDL_Event event;
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
        
        for (auto obj : _update.objects)
        {
            obj->update(dt);
        }
        render.update(dt, &_all.objects);
        
        // char buffer[64];
        // int ret = snprintf(buffer, sizeof buffer, "%f", dt);
        // SDL_SetWindowTitle(window, buffer);

        render.render();
        SDL_GL_SwapWindow(window);

        lastUpdate = current;
        Uint64 end = SDL_GetTicks();
        auto delay = end - start;
        if (delay < 16.666f)
        {
            SDL_Delay(16.666f - delay);
        }
    }
}