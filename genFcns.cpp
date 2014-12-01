#include "genFcns.h"

int getLinRnd(int min, int max) {
    static TCODRandom rnd = TCODRandom();
    return rnd.getInt(min,max);
}
