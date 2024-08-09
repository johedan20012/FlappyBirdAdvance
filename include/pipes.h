#ifndef PIPES_H
#define PIPES_H

#include "bn_array.h"
#include "bn_fixed_rect.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_pipe.h"

#include "global_stuff.h"

class PipePair{
private:
    bn::fixed half_gap;

    bn::sprite_ptr top_sprs[2];
    bn::sprite_ptr bottom_sprs[2];
public:
    PipePair(bn::fixed_point position, bn::fixed _gap = 40);
    ~PipePair() = default;

    bn::fixed x() const;
    void set_x(bn::fixed x);
    void set_y(bn::fixed y);

    bool check_collision(const bn::fixed_rect& bird_hitbox);
    bool passed(const bn::fixed_rect& bird_hitbox);
};

class Pipes {
private:
    GlobalStuff &global;

    bn::array<PipePair, 4> pipes;
    int current_pipe;
public:
    Pipes(GlobalStuff &_global);
    ~Pipes() = default;

    void update();

    bool check_collision(const bn::fixed_rect& bird_hitbox);
    bool check_passed(const bn::fixed_rect& bird_hitbox);
private:
    void set_random_y(int index);
};  

#endif