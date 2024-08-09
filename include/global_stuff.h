#ifndef GLOBAL_STUFF_H
#define GLOBAL_STUFF_H

#include "bn_random.h"

class GlobalStuff{
private:
    bn::random _rng;
    int _score;

public:
    GlobalStuff();
    ~GlobalStuff() = default;

    bn::random& rng();

    int score() const;
    void add_point();
    void reset_score();
};

#endif