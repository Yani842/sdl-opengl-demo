#include "Game.h"

int main(int argc, char *args[])
{
    Level level;
    level.init("attempting to add opengl", 1024, 768);
    level.run();
    return 0;
}