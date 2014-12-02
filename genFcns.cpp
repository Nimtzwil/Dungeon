#include "genFcns.h"

//allows the rnd # gen to be static across the entire program
int getLinRnd(int min, int max) {
    static TCODRandom rnd = TCODRandom();
    return rnd.getInt(min,max);
}
