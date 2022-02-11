#include "Game.h"

Object::Object() {}
Object::~Object() {}
void Object::init() {}
void Object::update(float dt) {}

NoLogicObject::NoLogicObject(AnimationEnum an, int x, int y, int h, int w)
{
    anState.setAnimation(an);
    anState.rect = {x, y, h, w};
}