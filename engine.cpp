#include "libtcod.hpp"
#include "actor.h"
#include "map.h"
#include "gui.h"
#include "engine.h"

Engine::Engine() {
    TCODConsole::initRoot(81,49,"Dungeon",false);
    player = new Actor(39,25,'@',TCODColor::lightAmber,0,1);
    actors.insertBefore(player,0);
    //insertBefore (___,0) will put the actor in the first spot so player (pos last) gets rendered last thus on top
    actors.insertBefore(new Actor(10,10,25,TCODColor::blue,1,1),0);
    actors.insertBefore(new Actor(30,29,140,TCODColor::yellow,3,1),0);
    map = new Map(81,45);
    gui = new GUI();
}

Engine::~Engine() {
    actors.clearAndDelete();
    delete map;
    delete gui;
}

void Engine::update() {	//checks for key presses
    TCOD_key_t key; //= TCODConsole::waitForKeypress(true);
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
    switch(key.vk) {
        case TCODK_UP :		//up
            //if ( ! map->isWall(player->x,player->y-1)) {
                player->y--;
                updateActors();
            //}
            player->facing = 0;
        break;
        case TCODK_DOWN :	//down
            //if ( ! map->isWall(player->x,player->y+1)) {
                player->y++;
                updateActors();
            //}
            player->facing = 2;
        break;
        case TCODK_LEFT :	//left
            //if ( ! map->isWall(player->x-1,player->y)) {
                player->x--;
                updateActors();
            //}
            player->facing = 3;
        break;
        case TCODK_RIGHT :	//right
            //if ( ! map->isWall(player->x+1,player->y)) {
                player->x++;
                updateActors();
            //}
            player->facing = 1;
        break;

        case TCODK_CHAR :	//not a special character
            if(key.c == 'p'){   //toggle stinkyness/color
                if (player->col == TCODColor::lightAmber){
                    player->col = TCODColor::darkGreen;
                    player->stinks = true;
                    actors.insertBefore(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,11),0);
                }
                else if (player->col == TCODColor::darkGreen){
                    player->col = TCODColor::lightAmber;
                    player->stinks = false;
                }
            }
            else if(key.c == 'g'){
                map->Daedalus();
            }
        break;

        default:break;	//default
    }

    map->newView(player->x, player->y, player->facing);	//recalculate what is currently seen
}

void Engine::updateActors(){
    map->moves++;

    if(player->stinks)    //add new scent actor if stinky
        actors.insertBefore(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,12),0);

    //actors.end()-1 wont iterate to player that always stays at the end
    for (Actor **iterator=actors.begin(); iterator != actors.end()-1; iterator++) {
    	//resets map when reach drop down
        if((player->x==(*iterator)->x)&&(player->y==(*iterator)->y)&&((*iterator)->type == 1)&&(map->proceed == true)){
            map->Daedalus();

            map->proceed = false;
        
            TCODRandom *rnd = new TCODRandom();
                    //put dropdown to a random position
            (*iterator)->x = (rnd->getInt(0,39)*2)+1;
            (*iterator)->y = (rnd->getInt(0,21)*2)+1;

            actors.insertBefore(new Actor((rnd->getInt(0,39)*2)+1,(rnd->getInt(0,21)*2)+1, 140, TCODColor::yellow, 3, 1), 0);

            //TODO check the key and drop down cant spawn in same place
            delete rnd;

            for (Actor **iterator2=actors.begin(); iterator2 != actors.end(); iterator2++) {    //kills all scent on dropdown
                if((*iterator2)->type == 2)
                    (*iterator2)->hp = 0;
            }
            
            if(player->stinks)    //add new scent actor if stinky
                actors.insertBefore(new Actor(player->x,player->y, 7, TCODColor::chartreuse, 2,11),0); //adds scent actor where fallen
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

    for (Actor **iterator=actors.begin(); iterator != actors.end(); iterator++) {
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
    for (Actor **iterator=actors.begin();
        iterator != actors.end(); iterator++) {
        (*iterator)->render();
    }
    gui->render(player->stinks,map->level,map->proceed,map->moves);
}
