#ifndef GLOBAL_STUFF_H
#define GLOBAL_STUFF_H

#include "bn_random.h"

class GlobalStuff{
private:
    bn::random _rng;

public:
    GlobalStuff();
    ~GlobalStuff() = default;

    bn::random& rng();
};

#endif