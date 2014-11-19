#ifndef actor_H
#define actor_H

#include "inventory.h"

class Actor{
public:
    int x,y;	//map position
    int ch;	//ascii code
    int facing;	//0=up 1=right 2=down 3=left
    int type;
    TCODColor col;	//color
    int hp;
    bool stinks; //determines if a scent path forms/stops
    Inventory *inv;

    Actor(int x, int y, int ch, const TCODColor &col, int typ, int health);
    ~Actor();
    void render() const;
};
#endif
