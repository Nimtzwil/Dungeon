#ifndef actor_H
#define actor_H

#include "inventory.h"

class Actor{
public:
//map position
    int x,y;
//ascii code
    int ch;
//0=up 1=right 2=down 3=left
    int facing;
    int type;
    TCODColor col;
    int hp;
//determines if a scent path forms/stops
    bool stinks;
    Inventory *inv;

    Actor(int x, int y, int ch, const TCODColor &col, int typ, int health);
    ~Actor();
    void render() const;
};
#endif
