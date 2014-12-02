#ifndef inventory_H
#define inventory_H

#include <list>
#include <string>
#include <vector>

#include "libtcod.hpp"

struct Loot{
    int placeholder;
    std::string name;

    Loot();

//creates a random name for the loot
    void createName();
};

class Inventory{
public:
    std::list<Loot *> knapsack;

    Inventory();
    ~Inventory();
};
#endif
