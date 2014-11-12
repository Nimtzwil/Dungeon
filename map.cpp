#include <cmath>

#include "libtcod.hpp"
#include "map.h"

Tile::Tile(){
    canWalk = true;
    Seen = false;
    Infered = true;
}

Room::Room(){
    Ux =0;
    Uy =0;
    Lx =0;
    Ly =0;
    exists = false;
}

Map::Map(int width, int height) : width(width),height(height) {
    tiles=new Tile[width*height];
    level = 0;
    moves = 0;
    proceed = false;
    maxDepth = 5;
    maxRooms = std::pow(2, maxDepth+1) - 1;
    rooms=new Room[maxRooms];
    for(int i=0; i < width; i++){	//sets up outer walls on edge of screen
        setWall(i,0);
        setWall(i,height-1);
    }
    for(int j=1; j < height-1; j++){
        setWall(0,j);
        setWall(width-1,j);
    }
    Daedalus();
}

Map::~Map() {
    delete [] rooms;
    delete [] tiles;
}

bool Map::isWall(int x, int y) const {	//is there a wall here?
    return !tiles[x+y*width].canWalk;
}
 
void Map::setWall(int x, int y) {	//create wall here
    tiles[x+y*width].canWalk=false;
}

bool Map::isSeen(int x, int y) const {	//tile has been seen
    return tiles[x+y*width].Seen;
}

void Map::wasSeen(int x, int y) {	//tile was just seen
    tiles[x+y*width].Seen=true;
}

bool Map::isInfered(int x, int y) const {	//tile has been infered
    return tiles[x+y*width].Infered;
}

void Map::wasInfered(int x, int y) {	//tile was just infered
    tiles[x+y*width].Infered=true;
}

void Map::newView(int x, int y, int fac) {
    if(fac == 0){	//reveals all that are above
        wasSeen(x-1,y);
        wasSeen(x+1,y);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x,y-1);
            wasInfered(x-1,y-1);
            wasInfered(x+1,y-1);
            y--;
        }
    }
    else if(fac == 1){	//reveals all that are right
        wasSeen(x,y-1);
        wasSeen(x,y+1);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x+1,y);
            wasInfered(x+1,y-1);
            wasInfered(x+1,y+1);
            x++;
        }
    }
    else if(fac == 2){	//reveals all that are below
        wasSeen(x-1,y);
        wasSeen(x+1,y);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x,y+1);
            wasInfered(x-1,y+1);
            wasInfered(x+1,y+1);
            y++;
        }
    }
    else if(fac == 3){	//reveals all that are left
        wasSeen(x,y-1);
        wasSeen(x,y+1);
        while(!isWall(x,y)){
            wasSeen(x,y);
            wasSeen(x-1,y);
            wasInfered(x-1,y-1);
            wasInfered(x-1,y+1);
            x--;
        }
    }
}

void Map::render() const {
    static const TCODColor seenWall(31,24,15);
    static const TCODColor inferedGround(50,50,50);
    static const TCODColor seenGround(100,100,100);
    for (int x=0; x < width; x++) {
        for (int y=0; y < height; y++) {
            if (isSeen(x,y))
                TCODConsole::root->setCharBackground(x,y,isWall(x,y) ? seenWall : seenGround );
            else if (isInfered(x,y))
                TCODConsole::root->setCharBackground(x,y,isWall(x,y) ? seenWall : inferedGround );
        }
    }
}

void Map::Daedalus() {	//builds a dungeon
    level++;
    moves = 0;

    for (int i=0; i <= width-1; i++){
        for (int j=0; j <= height-1; j++){
            tiles[i+j*width].Seen=false;	//for resetting the map
            tiles[i+j*width].Infered=true;
            tiles[i+j*width].canWalk = false;
        }
    }

    for (int k=0; k <= maxRooms-1; k++){    //"resets" rooms
        rooms[k].exists = false;
    }

    TCODRandom *rnd = new TCODRandom();
    int inindex = 1;
    Partition(0,0,width-1,height-1,0,rnd, inindex);
    delete rnd;
}

void Map::Partition(int ULx, int ULy, int LRx, int LRy, int depth, TCODRandom *rnd, int index){
    int split = 0;
    int breakout = 0;
    int minSize = 8;    //appears to work like minSize+1 (eg. want 7 use 8)

    if (depth < maxDepth){ //check if depth is max if not go deeper
        if ((LRx-ULx)>(LRy-ULy)){   //split along x
            split = rnd->getInt(ULx,LRx);

            while((breakout<50)&&(((split-ULx)<minSize)||((LRy-ULy)<minSize)||((LRx-split+1)<minSize)||((LRy-ULy)<minSize))){
                breakout++; //attempt to find a valid split line
                split = rnd->getInt(ULx,LRx);
            }

            if (breakout < 50){ //if valid line found recurse
                Partition(ULx,ULy,split,LRy,depth+1,rnd,2*index);
                Partition(split+1,ULy,LRx,LRy,depth+1,rnd,2*index+1);
            }
            else{   //else tell to build room
                Partition(ULx,ULy,LRx,LRy,maxDepth,rnd,index);
            }
        }
        else{   //split along y
            split = rnd->getInt(ULy,LRy);

            while((breakout<50)&&(((LRx-ULx)<minSize)||((split-ULy)<minSize)||((LRx-ULx)<minSize)||((LRy-split+1)<minSize))){
                breakout++; //attempt to find a valid split line
                split = rnd->getInt(ULy,LRy);
            }

            if (breakout < 50){ //if valid line found recurse
                Partition(ULx,ULy,LRx,split,depth+1,rnd,2*index);
                Partition(ULx,split+1,LRx,LRy,depth+1,rnd,2*index+1);
            }
            else{   //else tell to build room
                Partition(ULx,ULy,LRx,LRy,maxDepth,rnd,index);
            }
        }
    }
    else{   //else create rooms
        rooms[index-1].exists=true;
        rooms[index-1].Ux=rnd->getInt(ULx+2,(LRx-ULx)/2 +ULx-2);
        rooms[index-1].Uy=rnd->getInt(ULy+2,(LRy-ULy)/2 +ULy-2);
        rooms[index-1].Lx=rnd->getInt((LRx-ULx)/2 +ULx+2,LRx-2);
        rooms[index-1].Ly=rnd->getInt((LRy-ULy)/2 +ULy+2,LRy-2);
        
        for (int i=rooms[index-1].Ux; i <= rooms[index-1].Lx; i++){
            for (int j=rooms[index-1].Uy; j <= rooms[index-1].Ly; j++){
                tiles[i+j*width].canWalk = false;
            }
        }
    }
}
