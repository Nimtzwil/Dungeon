#include <list>
#include <utility>

#include "libtcod.hpp"
#include "actor.h"
#include "map.h"
#include "gui.h"
#include "engine.h"
#include "genFcns.h"

Engine::Engine() {
    TCODConsole::initRoot(81,49,"Dungeon",false);
    map = new Map(81,45);
    gui = new GUI();
    initializeMap();
}

Engine::~Engine() {
    for(auto each : actors){
        delete each;
    }
    delete map;
    delete gui;
}

//checks for key presses and updates accordingly
void Engine::update() {
    TCOD_key_t key;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key,NULL);
    switch(key.vk) {
//up
        case TCODK_UP :
            if ( ! map->isWall(player->x,player->y-1)) {
                player->y--;
                updateActors();
            }
            player->facing = 0;
        break;
//down
        case TCODK_DOWN :
            if ( ! map->isWall(player->x,player->y+1)) {
                player->y++;
                updateActors();
            }
            player->facing = 2;
        break;
//left
        case TCODK_LEFT :
            if ( ! map->isWall(player->x-1,player->y)) {
                player->x--;
                updateActors();
            }
            player->facing = 3;
        break;
//right
        case TCODK_RIGHT :
            if ( ! map->isWall(player->x+1,player->y)) {
                player->x++;
                updateActors();
            }
            player->facing = 1;
        break;

//not a special character
        case TCODK_CHAR :
//toggle stinkyness/color
            if(key.c == 'p'){
                if (player->col == TCODColor::lightAmber){
                    player->col = TCODColor::darkGreen;
                    player->stinks = true;
                    actors.push_front(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,11));
                }
                else if (player->col == TCODColor::darkGreen){
                    player->col = TCODColor::lightAmber;
                    player->stinks = false;
                }
            }
            else if(key.c == 'g'){
                map->BSPDaedalus();
            }
        break;

//default
        default:break;
    }

//recalculate what is currently seen
    map->newView(player->x, player->y, player->facing);
}

void Engine::updateActors(){
    map->moves++;

//add new scent actor if stinky
    if(player->stinks)
        actors.push_front(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,12));

//iterates through all actors to check for collisions with player
    for (auto iterator=actors.begin(); iterator != actors.end(); iterator++) {
//resets map when reach drop down
        if((player->x==(*iterator)->x)&&(player->y==(*iterator)->y)&&((*iterator)->type == 1)&&(map->proceed == true)){
            map->BSPDaedalus();

            map->proceed = false;

            std::pair<int,int> position;

//kills all scent&chests on dropdown
            for (auto iterator2=actors.begin(); iterator2 != actors.end(); iterator2++) {    
                if(((*iterator2)->type == 2)||((*iterator2)->type == 4))
                    (*iterator2)->hp = 0;
            }

//put player in a random position
            position = map->findValidPos();
            player->x = position.first;
            player->y = position.second;

//put dropdown to a random position
            position = map->findValidPos();
            (*iterator)->x = position.first;
            (*iterator)->y = position.second;

//put a key in random position
            position = map->findValidPos();
            actors.push_front(new Actor(position.first, position.second, 140, TCODColor::yellow, 3, 1));

//place chests
            populateMap();

            //TODO check the key and drop down cant spawn in same place

            if(player->stinks)
//adds scent actor where fallen
                actors.push_front(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,11));
//prevents seg fault after pushing the new actor
                break;
        }

//if key is found
        else if((player->x==(*iterator)->x)&&(player->y==(*iterator)->y)&&((*iterator)->type == 3)){
            map->proceed = true;
            (*iterator)->hp = 0;
        }

//if chest is found
        else if((player->x==(*iterator)->x)&&(player->y==(*iterator)->y)&&((*iterator)->type == 4)){
            (*iterator)->hp = 0;
            player->inv = (*iterator)->inv;
        }

//decrements "scent" health
        else if((*iterator)->type == 2){
            (*iterator)->hp--;
        }
    }

//checks if each actor is dead
    for (auto iterator=actors.begin(); iterator != actors.end(); iterator++) {
        if((*iterator)->hp < 1){           
            actors.remove((*iterator));
            iterator--;
        }
//breaks loop to prevent seg fault
        if(iterator == actors.end())
            break;
    }
}

void Engine::render() {
    TCODConsole::root->clear();
// draw the map
    map->render();
// draw the actors
    for (auto iterator=actors.begin(); iterator != actors.end(); iterator++) {
        (*iterator)->render();
    }
    gui->render(player->stinks,map->level,map->proceed,map->moves, player->inv->knapsack.front()->name);
}

void Engine::initializeMap() {
    std::pair<int,int> position;

//place player
    position = map->findValidPos();
    player = new Actor(position.first, position.second,'@',TCODColor::lightAmber,0,1);
    actors.push_front(player);

//place door
    position = map->findValidPos();
    actors.push_front(new Actor(position.first, position.second,25,TCODColor::blue,1,1));
    
//place key
    position = map->findValidPos();
    actors.push_front(new Actor(position.first, position.second,140,TCODColor::yellow,3,1));

//place else
    populateMap();
}

void Engine::populateMap() {
    std::pair<int,int> position2;

//places a number of chests
    int cntMax = getLinRnd(4,9);
    for (int cnt = 0; cnt < cntMax; cnt++){
        position2 = map->findValidPos();
        actors.push_front(new Actor(position2.first, position2.second,8,TCODColor::darkFlame,4,1));
    }
}
