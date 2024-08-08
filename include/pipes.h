#ifndef PIPES_H
#define PIPES_H

#include "bn_array.h"
#include "bn_fixed_rect.h"

#include "bn_sprite_ptr.h"
#include "bn_sprite_items_pipe.h"

class Pipe {
private:
    bn::sprite_ptr sprs[2];
    bool flipped;

public:
    Pipe(bool _flipped, bn::fixed_point position);
    ~Pipe() = default;

    bn::fixed x() const;
    void set_x(bn::fixed x);
    void set_pos(bn::fixed_point position);

    bn::fixed_rect hitbox() const;
};

class Pipes {
private:
    bn::array<Pipe, 8> pipes;

public:
    Pipes();
    ~Pipes() = default;

    void update();

    bool check_collision(const bn::fixed_rect& bird_hitbox);

private:
    void reset_pipe(int index);
};  

#endif