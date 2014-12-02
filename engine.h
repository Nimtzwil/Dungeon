#ifndef engine_H
#define engine_H

#include <list>

#include "actor.h"
#include "map.h"
#include "gui.h"

class Engine {
public :
    std::list<Actor *> actors;
    Actor *player;
    Map *map;
    GUI *gui;
 
    Engine();
    ~Engine();
    void update();
    void updateActors();
    void render();
    void initializeMap();
    void populateMap();
};
 
extern Engine engine;
#endif
