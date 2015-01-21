#include "libtcod.hpp"
#include "actor.h"
#include "inventory.h"

Actor::Actor(int x, int y, int ch, const TCODColor &col, int typ, int health):
    x(x),y(y),ch(ch),col(col) {
facing = 0;
type = typ;
hp = health;
stinks = false;
delay = 100;
inv = new Inventory();
}

Actor::~Actor(){
    delete inv;
}

void Actor::render() const{
    TCODConsole::root->setChar(x,y,ch);
    TCODConsole::root->setCharForeground(x,y,col);
}
