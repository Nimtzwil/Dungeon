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
    //std::string line = "";

    //if(styles.empty()){
    //    std::ifstream in1;
    //    in1.open("astyle.txt", std::ifstream::in);
    //    while(std::getline(in1,line)) {
    //        styles.push_back(line);
//std::cout<<line<<"jjj"<<std::endl;
	//    }
    //    in1.close();
    //}

    //if(types.empty()){
    //    std::ifstream in2("btype.txt");
    //    while(getline(in2,line)) {
    //        types.push_back(line);
	//    }
    //}
//std::cout<<"HERE"<<std::endl;
    //line = styles.front();
//std::cout<<"HERE2"<<std::endl;
    //name = line;
    //name =" IJ";
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
