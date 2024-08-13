#ifndef GLOBAL_STUFF_H
#define GLOBAL_STUFF_H

#include "bn_random.h"

#include "bn_sprite_items_huge_numbers_font.h"
#include "bn_sprite_items_numbers_font.h"
#include "bn_sprite_items_small_numbers_font.h"

#include "save.h"
#include "text_number_generator.h"

class GlobalStuff{
private:
    Save save;

    bool portrait_mode;
    bn::random _rng;
    int _score;

    TextNumberGenerator huge_generator;
    TextNumberGenerator normal_generator;
    TextNumberGenerator small_generator;
public:
    GlobalStuff();
    ~GlobalStuff() = default;

    Save& save_data();

    bool is_portrait_mode() const;
    void toggle_portrait_mode();

    bn::random& rng();

    int score() const;
    void add_point();
    void reset_score();

    TextNumberGenerator& huge_gen();
    TextNumberGenerator& normal_gen();
    TextNumberGenerator& small_gen();

};

#endif