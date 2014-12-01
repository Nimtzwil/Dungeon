#include <list>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "libtcod.hpp"
#include "inventory.h"

Loot::Loot(){
    placeholder = 0;
    name = "UN-NAMED";
}

void Loot::createName(){
    static std::vector<std::string> styles = {"Iron", "Steel", "Orcish", "Dwarven", "Nord Hero", "Skyforge Steel", "Elven", "Nordic", "Glass", "Ebony", "Stalhrim", "Daedric", "Dragonbone"};
    static std::vector<std::string> types = {"Sword", "War Axe", "Mace", "Dagger", "Greatsword", "Battleaxe", "Warhammer", "Bow", "Crossbow"};
    static TCODRandom rnd = TCODRandom();

    name = styles[rnd.getInt(0,styles.size()-1)];
    name += " ";
    name += types[rnd.getInt(0,types.size()-1)];

    //delete rnd;
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
