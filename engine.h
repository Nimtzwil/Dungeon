#include <list>

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
    void populateMap();
};
 
extern Engine engine;
