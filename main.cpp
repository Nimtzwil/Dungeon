#include "libtcod.hpp"
#include "actor.h"
#include "map.h"
#include "gui.h"
#include "engine.h"
 
Engine engine;
 
int main() {
    while ( !TCODConsole::isWindowClosed() ) {
        engine.update();
        engine.render();
        TCODConsole::flush();
    }
    return 0;
}
