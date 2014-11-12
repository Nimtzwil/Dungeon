#include "libtcod.hpp"
#include "actor.h"

Actor::Actor(int x, int y, int ch, const TCODColor &col, int typ, int health):
    x(x),y(y),ch(ch),col(col) {
facing = 0;
type = typ;
hp = health;
stinks = false;
}

void Actor::render() const{
    TCODConsole::root->setChar(x,y,ch);
    TCODConsole::root->setCharForeground(x,y,col);
}
