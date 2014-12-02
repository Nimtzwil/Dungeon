#ifndef map_H
#define map_H

#include <list>
#include <utility>

#include "libtcod.hpp"

struct Tile {
// can we walk through this tile?
    bool canWalk;
// has been seen by player
    bool Seen;
// has been on edge of vision
    bool Infered;
    int occupied;

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
    void setWall(int x, int y);

    bool isSeen(int x, int y) const;
    void wasSeen(int x,int y);
    void wasBlocked(int x, int y);

    bool isInfered(int x, int y) const;
    void wasInfered(int x, int y);

    void render() const;
//updates view
    void newView(int x, int y, int fac);

    void makeHallway(int Sx, int Sy, int Ex, int Ey);
    void makeRoom(int LLx, int LLy, int Rwidth, int height);

//finds a random non-wall pt to return
    std::pair<int,int> findValidPos();

//builds a dungeon
    void BSPDaedalus();
    void BSPPartition(int ULx, int ULy, int LRx, int LRy, int depth, int index);
    std::list<int> BSPconnectChildren(int index);

private:
    Tile *tiles;
    Room *BSProoms;
};
#endif
