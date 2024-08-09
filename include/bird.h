#ifndef BIRD_H
#define BIRD_H 

#include "bn_keypad.h"
#include "bn_fixed_rect.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_animate_actions.h"
#include "bn_sprite_items_bird.h"

#include "bn_sound_items.h"

class Bird {
private:
    static constexpr bn::fixed FLOOR_Y = 58; // 80 -16 (floor) - 6 (bird)
    static constexpr bn::fixed GRAVITY = 0.096; // Pixels/Frames^2 converted from 7.63 m/s^2 assuming 1m=48px and 1s=60frames
    static constexpr bn::fixed JUMP_VELOCITY = -1.85;
    static constexpr bn::fixed BIRD_WIDTH = 8;
    static constexpr bn::fixed BIRD_HEIGHT = 8;

    bn::sprite_ptr spr;
    bn::sprite_animate_action<4> flap_action;

    bn::fixed y_velocity;
    bn::fixed angle;

    bool idle;
    bool dead;

public:
    Bird();
    ~Bird() = default;

    void update();

    bool is_idle() const;
    void activate();

    void kill();
    bool is_dead() const;

    bool is_on_floor() const;

    bn::fixed_rect hitbox() const;
};

#endif