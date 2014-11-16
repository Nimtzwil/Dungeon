#include "libtcod.hpp"

struct Tile {
    bool canWalk; 	// can we walk through this tile?
    bool Seen;		// has been seen by player
    bool Infered;	// has been on edge of vision
    Tile();
};

struct Room {
    int Ux;
    int Uy;
    int Lx;
    int Ly;
    bool exists;
    Room();
};
 
class Map {
public:
    int width,height;
    int level;
    int moves;
    bool proceed;
    int maxDepth;
    int maxRooms;
 
    Map(int width, int height);
    ~Map();
    bool isWall(int x, int y) const;
    bool isSeen(int x, int y) const;
    void wasSeen(int x,int y);
    bool isInfered(int x, int y) const;
    void wasInfered(int x, int y);
    void render() const;
    void newView(int x, int y, int fac);    //updates view
    void Daedalus();	//builds a dungeon
    void Partition(int ULx, int ULy, int LRx, int LRy, int depth, TCODRandom *rnd, int index);
    void makeHallway(int Sx, int Sy, int Ex, int Ey);
    void connectRooms();
private:
    Tile *tiles;
    Room *rooms;
 
    void setWall(int x, int y);
};
