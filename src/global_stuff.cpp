#include "global_stuff.h"

GlobalStuff::GlobalStuff() : 
    save(),
    _score(11),
    huge_generator(bn::sprite_items::huge_numbers_font,11),
    normal_generator(bn::sprite_items::numbers_font),
    small_generator(bn::sprite_items::small_numbers_font,6)
    {
    save.load_from_sram();
}

Save& GlobalStuff::save_data(){
    return save;
}

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

TextNumberGenerator& GlobalStuff::huge_gen(){
    return huge_generator;
}

TextNumberGenerator& GlobalStuff::normal_gen(){
    return normal_generator;
}

TextNumberGenerator& GlobalStuff::small_gen(){
    return small_generator;
}