#include "global_stuff.h"

GlobalStuff::GlobalStuff() : 
    _score(0) {}

bn::random& GlobalStuff::rng(){
    return _rng;
}

int GlobalStuff::score() const{
    return _score;
}

void GlobalStuff::add_point(){
    _score++;
}

void GlobalStuff::reset_score(){
    _score = 0;
}

