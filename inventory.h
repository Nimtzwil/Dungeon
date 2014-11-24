#ifndef inventory_H
#define inventory_H

#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "libtcod.hpp"

struct Loot{
    int placeholder;
    std::string name;

    Loot();

    void createName();
};

class Inventory{
public:
    std::list<Loot *> knapsack;

    Inventory();
    ~Inventory();
};
#endif
