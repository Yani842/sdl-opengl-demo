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
    // if there will be more lists add them here
}

void Level::init(SDL_Renderer* renderer) {}

void Level::update(float dt)
{
    for (auto obj : _update.objects)
    {
        obj->update(dt);
    }
    render.update(dt, &_all.objects);
}

void LevelStart::init(SDL_Renderer* renderer)
{
    render.init(renderer);
    render.levelStartLoadAnimations();
    
    shared_ptr<Object> pl (new Player());
    _all.add(pl);
    _update.add(pl);
    _top.add(pl);
    pl->init();

    shared_ptr<Object> gr1 (new NoLogicObject(grass1, 0, 200, 60, 60));
    _all.add(gr1);
    _ground.add(gr1);
    shared_ptr<Object> gr2 (new NoLogicObject(grass2, 60, 200, 60, 60));
    _all.add(gr2);
    _ground.add(gr2);
    shared_ptr<Object> gr3 (new NoLogicObject(grass3, 120, 200, 60, 60));
    _all.add(gr3);
    _ground.add(gr3);
    shared_ptr<Object> gr4 (new NoLogicObject(grass4, 180, 200, 60, 60));
    _all.add(gr4);
    _ground.add(gr4);
    shared_ptr<Object> gr5 (new NoLogicObject(grass5, 240, 200, 60, 60));
    _all.add(gr5);
    _ground.add(gr5);
    shared_ptr<Object> gr6 (new NoLogicObject(grassWithWater, 300, 200, 60, 60));
    _all.add(gr6);
    _ground.add(gr6);
    shared_ptr<Object> tr1 (new NoLogicObject(tree1, 100, 200, 45*4, 75*4));
    _all.add(tr1);
    _foliage.add(tr1);
    shared_ptr<Object> tr2 (new NoLogicObject(tree2, 300, 200, 45*4, 75*4));
    _all.add(tr2);
    _foliage.add(tr2);
}

void LevelStart::draw()
{
    render.render(&_foliage.objects);
    render.render(&_ground.objects);
    render.render(&_top.objects);
}