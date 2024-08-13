#ifndef BIRD_H
#define BIRD_H 

#include "bn_keypad.h"
#include "bn_fixed_rect.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_bird.h"

#include "bn_sound_items.h"

#include "global_stuff.h"

class Bird {
private:
    static constexpr bn::fixed FLOOR_LEVEL_LANDSCAPE = 58; // 80 -16 (floor) - 6 (bird)
    static constexpr bn::fixed FLOOR_LEVEL_PORTRAIT = -52; // -120 +64 (floor) + 6 (bird)
    static constexpr bn::fixed GRAVITY = 0.096; // Pixels/Frames^2 converted from 7.63 m/s^2 assuming 1m=48px and 1s=60frames
    static constexpr bn::fixed JUMP_VELOCITY = -1.85;
    static constexpr bn::fixed BIRD_WIDTH = 8;
    static constexpr bn::fixed BIRD_HEIGHT = 8;

    bool portrait_mode;

    bn::fixed iddle_offset;

    bn::sprite_ptr spr;
    bn::sprite_animate_action<4> flap_action;

    bn::fixed moving_axis_velocity;
    bn::fixed angle;

    bool idle;
    bn::fixed dir = 0.125;
    bool dead;

public:
    Bird(GlobalStuff& _global,int color = 0,bn::fixed _static_axis_pos = -64, bn::fixed _moving_axis_offset = 0);
    ~Bird() = default;

    void update();

    bool is_idle() const;
    void activate();

    void kill(bool hit_pipe = false);
    bool is_dead() const;

    bool is_on_floor() const;

    bn::fixed_rect hitbox() const;
};

#endif