#include <list>
#include <string>

#include "inventory.h"

Loot::Loot(){
    placeholder = 0;
    name = "UN-NAMED";
}

void Loot::createName(){
    name = "generic sword";
}

Inventory::Inventory(){
    Loot *generic = new Loot();
    generic->createName();

    knapsack.push_back(generic);
}

Inventory::~Inventory(){
    for(auto each : knapsack){
        delete each;
    }
}
