#ifndef gui_H
#define gui_H

#include <string>

class GUI{
public:
    TCODColor borderColor;

    GUI();

//renders the given info
    void render(bool smell,int level, bool proceed, int moves, std::string equipped) const;
};
#endif
