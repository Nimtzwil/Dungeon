#ifndef gui_H
#define gui_H
class GUI{
public:
    TCODColor borderColor;

    GUI();
    void render(bool smell,int level, bool proceed, int moves) const;
};
#endif
