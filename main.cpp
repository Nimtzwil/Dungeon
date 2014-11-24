#include "libtcod.hpp"
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
