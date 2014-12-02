#include <list>
#include <string>
#include <vector>

#include "libtcod.hpp"
#include "inventory.h"
#include "genFcns.h"

Loot::Loot(){
    placeholder = 0;
    name = "UN-NAMED";
}

//creates a random name for the Loot
void Loot::createName(){
    static std::vector<std::string> styles = {"Iron", "Steel", "Orcish", "Dwarven", "Nord Hero", "Skyforge Steel", "Elven", "Nordic", "Glass", "Ebony", "Stalhrim", "Daedric", "Dragonbone"};
    static std::vector<std::string> types = {"Sword", "War Axe", "Mace", "Dagger", "Greatsword", "Battleaxe", "Warhammer", "Bow", "Crossbow"};

    name = styles[getLinRnd(0,styles.size()-1)];
    name += " ";
    name += types[getLinRnd(0,types.size()-1)];
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
