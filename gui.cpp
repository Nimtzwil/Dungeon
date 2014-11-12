#include "libtcod.hpp"
#include "gui.h"

#include <string>

GUI::GUI(){
    misc = 42;
    borderColor = TCODColor::darkRed;
}

void GUI::render(bool smell, int level, bool proceed, int moves) const{

    //render edges in red
    TCODConsole::root->setChar(0,45,201);
    TCODConsole::root->setCharForeground(0,45,borderColor);
    TCODConsole::root->setChar(0,48,200);
    TCODConsole::root->setCharForeground(0,48,borderColor);
    TCODConsole::root->setChar(80,45,187);
    TCODConsole::root->setCharForeground(80,45,borderColor);
    TCODConsole::root->setChar(80,48,188);
    TCODConsole::root->setCharForeground(80,48,borderColor);
    for(int i = 1 ; i < 80; i++){
        TCODConsole::root->setChar(i,45,205);
        TCODConsole::root->setCharForeground(i,45,borderColor);
        TCODConsole::root->setChar(i,48,205);
        TCODConsole::root->setCharForeground(i,48,borderColor);
    }
    for(int j = 46 ; j < 48; j++){
        TCODConsole::root->setChar(0,j,186);
        TCODConsole::root->setCharForeground(0,j,borderColor);
        TCODConsole::root->setChar(80,j,186);
        TCODConsole::root->setCharForeground(80,j,borderColor);
    }

    //render smelly status
    int count = 0;
    std::string str = "Smelly: ";
    for (char c: str){
        TCODConsole::root->setChar(1+count,47,c);
        TCODConsole::root->setCharForeground(1+count,47, TCODColor::chartreuse);
        count++;
    }

    if(smell)
        TCODConsole::root->setChar(1+count,47,89);
    else
        TCODConsole::root->setChar(1+count,47,78);
    TCODConsole::root->setCharForeground(1+count,47, TCODColor::chartreuse);
    count++;

    str = " (toggle w/ 'p')";
    for (char c: str){
        TCODConsole::root->setChar(1+count,47,c);
        TCODConsole::root->setCharForeground(1+count,47, TCODColor::chartreuse);
        count++;
    }

    //render proceed status
    str = "   Proceed? ";
    for (char c: str){
        TCODConsole::root->setChar(1+count,47,c);
        TCODConsole::root->setCharForeground(1+count,47, TCODColor::chartreuse);
        count++;
    }
    if(proceed)
        TCODConsole::root->setChar(1+count,47,225);
    else
        TCODConsole::root->setChar(1+count,47,224);
    TCODConsole::root->setCharForeground(1+count,47, TCODColor::chartreuse);
    count++;
    
    //render level info
    count = 0;
    str = "Level: ";
    for (char c: str){
        TCODConsole::root->setChar(1+count,46,c);
        TCODConsole::root->setCharForeground(1+count,46, TCODColor::chartreuse);
        count++;
    }

    str = std::to_string(level);
    for (char c: str){
        TCODConsole::root->setChar(1+count,46,c);
        TCODConsole::root->setCharForeground(1+count,46, TCODColor::chartreuse);
        count++;
    }

    str = "   Moves: ";
    for (char c: str){
        TCODConsole::root->setChar(1+count,46,c);
        TCODConsole::root->setCharForeground(1+count,46, TCODColor::chartreuse);
        count++;
    }

    str = std::to_string(moves);
    for (char c: str){
        TCODConsole::root->setChar(1+count,46,c);
        TCODConsole::root->setCharForeground(1+count,46, TCODColor::chartreuse);
        count++;
    }
}
