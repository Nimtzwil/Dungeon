#include <cmath>
#include <iostream>//debug
#include <list>

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
    BSProoms=new Room[maxRooms];
    for(int i=0; i < width; i++){	//sets up outer walls on edge of screen
        setWall(i,0);
        setWall(i,height-1);
    }
    for(int j=1; j < height-1; j++){
        setWall(0,j);
        setWall(width-1,j);
    }
    BSPDaedalus();
}

Map::~Map() {
    delete [] BSProoms;
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

void Map::BSPDaedalus() {	//builds a dungeon
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
        BSProoms[k].exists = false;
    }

    TCODRandom *rnd = new TCODRandom();
    int inindex = 1;
    BSPPartition(0,0,width-1,height-1,0,rnd, inindex);
    BSPconnectChildren(1);
    delete rnd;
}

void Map::BSPPartition(int ULx, int ULy, int LRx, int LRy, int depth, TCODRandom *rnd, int index){
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
                BSPPartition(ULx,ULy,split,LRy,depth+1,rnd,2*index);
                BSPPartition(split+1,ULy,LRx,LRy,depth+1,rnd,2*index+1);
            }
            else{   //else tell to build room
                BSPPartition(ULx,ULy,LRx,LRy,maxDepth,rnd,index);
            }
        }
        else{   //split along y
            split = rnd->getInt(ULy,LRy);

            while((breakout<50)&&(((LRx-ULx)<minSize)||((split-ULy)<minSize)||((LRx-ULx)<minSize)||((LRy-split+1)<minSize))){
                breakout++; //attempt to find a valid split line
                split = rnd->getInt(ULy,LRy);
            }

            BSProoms[index-1].Ux=ULx;
            BSProoms[index-1].Uy=ULy;
            BSProoms[index-1].Lx=LRx;
            BSProoms[index-1].Ly=LRy;

            if (breakout < 50){ //if valid line found recurse
                BSPPartition(ULx,ULy,LRx,split,depth+1,rnd,2*index);
                BSPPartition(ULx,split+1,LRx,LRy,depth+1,rnd,2*index+1);
            }
            else{   //else tell to build room
                BSPPartition(ULx,ULy,LRx,LRy,maxDepth,rnd,index);
            }
        }
    }
    else{   //else create rooms
        BSProoms[index-1].exists=true; //choose random size
        BSProoms[index-1].Ux=rnd->getInt(ULx+2,(LRx-ULx)/2 +ULx-2);
        BSProoms[index-1].Uy=rnd->getInt(ULy+2,(LRy-ULy)/2 +ULy-2);
        BSProoms[index-1].Lx=rnd->getInt((LRx-ULx)/2 +ULx+2,LRx-2);
        BSProoms[index-1].Ly=rnd->getInt((LRy-ULy)/2 +ULy+2,LRy-2);

        makeRoom(BSProoms[index-1].Ux, BSProoms[index-1].Uy, BSProoms[index-1].Lx, BSProoms[index-1].Ly);
        
    }
}

void Map::makeHallway(int Sx, int Sy, int Ex, int Ey){
    int dx = Ex-Sx;
    int dy = Ey-Sy;
    int x = Sx;
    int y = Sy;
    int diag = 0;
    int pastdiag = 0;
    int * dmaj = &dx;
    int * dmin = &dy;
    int * major = &x;
    int * minor = &y;
    int Smaj = Sx;
    
    tiles[x+y*width].canWalk = true;

//flips values if needed
    if (abs(dy)>abs(dx)){
        dmaj = &dy;
        dmin = &dx;
        major = &y;
        minor = &x;
        Smaj = Sy;
    }

//continues while not at destination
    while((x!=Ex)||(y!=Ey)){
        pastdiag = diag;
//determines value of next step
        diag = floor(abs((*dmin)*((*major)-(Smaj)+((*dmaj)/abs(*dmaj))))/(abs(*dmaj)));

        if(abs(diag) <= abs(pastdiag)){
//if delta value < 1 move along major axis
            (*major) = (*major)+((*dmaj)/abs(*dmaj));
            tiles[x+y*width].canWalk = true;
        }
        else{
//else move horizontal
            (*major) = (*major)+((*dmaj)/abs(*dmaj));
            tiles[x+y*width].canWalk = true;
            (*minor) = (*minor)+((*dmin)/abs(*dmin));
            tiles[x+y*width].canWalk = true;                
        }
    }
}

void Map::makeRoom(int LLx, int LLy, int URx, int URy){
    for (int u=LLx; u <= URx; u++){
        for (int t=LLy; t <= URy; t++){
            tiles[u+t*width].canWalk = true;
        }
    }
}

std::list<int> Map::BSPconnectChildren(int index){
//BSP indexing starts at 1 thus refs to rooms array are index-wanted-1
    std::list<int> Lchildren;
    std::list<int> Rchildren;
    int leftConnector;
    int rightConnector;
    int CRx, CRy, CLx, CLy;
    float curDis=0, minDis=0;

//creates list of "left" children and ensures they are merged
    if(BSProoms[2*index-1].exists)
        Lchildren.push_back(2*index);
    else
        Lchildren = BSPconnectChildren(2*index);

//creates list of "right" children and ensures they are merged
    if(BSProoms[2*index].exists)
        Rchildren.push_back(2*index+1);
    else
        Rchildren = BSPconnectChildren(2*index+1);

//looks through all pairs of left and right kids to find shortest path
    for(int l : Lchildren){
        for(int r : Rchildren){
            CRx = (int)((BSProoms[r-1].Lx-BSProoms[r-1].Ux)/2) + BSProoms[r-1].Ux;
            CRy = (int)((BSProoms[r-1].Ly-BSProoms[r-1].Uy)/2) + BSProoms[r-1].Uy;
            CLx = (int)((BSProoms[l-1].Lx-BSProoms[l-1].Ux)/2) + BSProoms[l-1].Ux;
            CLy = (int)((BSProoms[l-1].Ly-BSProoms[l-1].Uy)/2) + BSProoms[l-1].Uy;

            curDis = sqrt(pow(CRx-CLx,2)+pow(CRy-CLy,2));
            if((curDis<minDis)||(minDis==0)){
                minDis = curDis;
                leftConnector = l;
                rightConnector = r;
            }
        }
    }

//finallize shortest path measurements
    CRx = (int)((BSProoms[rightConnector-1].Lx-BSProoms[rightConnector-1].Ux)/2) + BSProoms[rightConnector-1].Ux;
    CRy = (int)((BSProoms[rightConnector-1].Ly-BSProoms[rightConnector-1].Uy)/2) + BSProoms[rightConnector-1].Uy;
    CLx = (int)((BSProoms[leftConnector-1].Lx-BSProoms[leftConnector-1].Ux)/2) + BSProoms[leftConnector-1].Ux;
    CLy = (int)((BSProoms[leftConnector-1].Ly-BSProoms[leftConnector-1].Uy)/2) + BSProoms[leftConnector-1].Uy;

//connect across path
    makeHallway(CRx,CRy,CLx,CLy);

//merge left and right lists to pass up
    Lchildren.merge(Rchildren);

    return Lchildren;
}
