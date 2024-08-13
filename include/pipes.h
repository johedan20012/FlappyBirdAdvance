#ifndef PIPES_H
#define PIPES_H

#include "bn_array.h"
#include "bn_fixed_rect.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_pipe.h"
#include "bn_sprite_items_pipe_portrait.h"

#include "global_stuff.h"

class PipePair{
private:
    bool portrait_mode;

    bn::fixed half_gap;

    bn::sprite_ptr top_sprs[2];
    bn::sprite_ptr bottom_sprs[2];
public:
    PipePair(bn::fixed_point position, bool _portrait_mode = false, bn::fixed _gap = 40);
    ~PipePair() = default;

    bn::fixed pos_moving_ax() const;
    void set_pos_moving_ax(bn::fixed pos);
    void set_pos_static_ax(bn::fixed pos);

    bool check_collision(const bn::fixed_rect& bird_hitbox);
    bool passed(const bn::fixed_rect& bird_hitbox);
};

class Pipes {
private:
    static constexpr bn::fixed_point PIPES_INITIAL_POS [2][4] = {
        {bn::fixed_point(136,-52), bn::fixed_point(216,-52), bn::fixed_point(296,-52), bn::fixed_point(376,-52)},
        {bn::fixed_point(10,96), bn::fixed_point(10,184), bn::fixed_point(10,272), bn::fixed_point(10,360)}
    };
    static constexpr bn::fixed MOVING_AX_LIMIT[2] = {bn::fixed(-183), bn::fixed(-255)};
    static constexpr bn::fixed MOVING_AX_RESET[2] = {bn::fixed(136), bn::fixed(96)};

    GlobalStuff &global;

    bool portrait_mode;

    bn::array<PipePair, 4> pipes;
    int current_pipe;
public:
    Pipes(GlobalStuff &_global);
    ~Pipes() = default;

    void update();

    bool check_collision(const bn::fixed_rect& bird_hitbox);
    bool check_passed(const bn::fixed_rect& bird_hitbox);
private:
    void set_random_static_ax(int index);
};  

#endif