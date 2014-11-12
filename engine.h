class Engine {
public :
    TCODList<Actor *> actors;
    Actor *player;
    Map *map;
    GUI *gui;
 
    Engine();
    ~Engine();
    void update();
    void updateActors();
    void render();
};
 
extern Engine engine;
