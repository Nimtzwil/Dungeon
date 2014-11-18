#include <list>
#include <utility>

#include "libtcod.hpp"
#include "actor.h"
#include "map.h"
#include "gui.h"
#include "engine.h"

Engine::Engine() {
    TCODConsole::initRoot(81,49,"Dungeon",false);
    map = new Map(81,45);
    gui = new GUI();
    populateMap();
}

Engine::~Engine() {
//memory leak??? may have to DELETE each actor b/c started w/ new?
    actors.clear();
    delete map;
    delete gui;
}

void Engine::update() {	//checks for key presses
    TCOD_key_t key; //= TCODConsole::waitForKeypress(true);
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
    switch(key.vk) {
        case TCODK_UP :		//up
            if ( ! map->isWall(player->x,player->y-1)) {
                player->y--;
                updateActors();
            }
            player->facing = 0;
        break;
        case TCODK_DOWN :	//down
            if ( ! map->isWall(player->x,player->y+1)) {
                player->y++;
                updateActors();
            }
            player->facing = 2;
        break;
        case TCODK_LEFT :	//left
            if ( ! map->isWall(player->x-1,player->y)) {
                player->x--;
                updateActors();
            }
            player->facing = 3;
        break;
        case TCODK_RIGHT :	//right
            if ( ! map->isWall(player->x+1,player->y)) {
                player->x++;
                updateActors();
            }
            player->facing = 1;
        break;

        case TCODK_CHAR :	//not a special character
            if(key.c == 'p'){   //toggle stinkyness/color
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

        default:break;	//default
    }

    map->newView(player->x, player->y, player->facing);	//recalculate what is currently seen
}

void Engine::updateActors(){
    map->moves++;

    if(player->stinks)    //add new scent actor if stinky
        actors.push_front(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,12));

    for (auto iterator=actors.begin(); iterator != actors.end(); iterator++) {
    	//resets map when reach drop down
        if((player->x==(*iterator)->x)&&(player->y==(*iterator)->y)&&((*iterator)->type == 1)&&(map->proceed == true)){
            map->BSPDaedalus();

            map->proceed = false;
        
            TCODRandom *rnd = new TCODRandom();
            std::pair<int,int> position;

//put player in a random position
            position = map->findValidPos(rnd);
            player->x = position.first;
            player->y = position.second;

//put dropdown to a random position
            position = map->findValidPos(rnd);
            (*iterator)->x = position.first;
            (*iterator)->y = position.second;

//put key in random position
            position = map->findValidPos(rnd);
            actors.push_front(new Actor(position.first, position.second, 140, TCODColor::yellow, 3, 1));

            //TODO check the key and drop down cant spawn in same place
            delete rnd;

            for (auto iterator2=actors.begin(); iterator2 != actors.end(); iterator2++) {    //kills all scent on dropdown
                if((*iterator2)->type == 2)
                    (*iterator2)->hp = 0;
            }
            
            if(player->stinks)    //add new scent actor if stinky
                actors.push_front(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,11)); //adds scent actor where fallen
                break;  //prevents seg fault after pushing the new actor
        }

        //if key is found
        else if((player->x==(*iterator)->x)&&(player->y==(*iterator)->y)&&((*iterator)->type == 3)){
            map->proceed = true;
            (*iterator)->hp = 0;
        }

        //decrements "scent" health
        else if((*iterator)->type == 2){
            (*iterator)->hp--;
        }
    }

    for (auto iterator=actors.begin(); iterator != actors.end(); iterator++) {
        if((*iterator)->hp < 1){   //checks if each actor is dead           
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
    gui->render(player->stinks,map->level,map->proceed,map->moves);
}

void Engine::populateMap() {
    TCODRandom *rnd = new TCODRandom();
    std::pair<int,int> position;

    position = map->findValidPos(rnd);
    player = new Actor(position.first, position.second,'@',TCODColor::lightAmber,0,1);
    actors.push_front(player);

    position = map->findValidPos(rnd);
    actors.push_front(new Actor(position.first, position.second,25,TCODColor::blue,1,1));
    
    position = map->findValidPos(rnd);
    actors.push_front(new Actor(position.first, position.second,140,TCODColor::yellow,3,1));

    delete rnd;
}
