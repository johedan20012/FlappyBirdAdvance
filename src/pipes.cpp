#include "pipes.h"

Pipe::Pipe(bool _flipped, bn::fixed_point position) :
    sprs{bn::sprite_items::pipe.create_sprite(position.x(), position.y()), bn::sprite_items::pipe.create_sprite(position.x(), position.y()+64,1)},
    flipped(_flipped)
    {
    if(flipped){
        sprs[0].set_vertical_flip(flipped);
        sprs[1].set_y(sprs[1].y() - 128);
    }

    sprs[0].set_bg_priority(2);
    sprs[1].set_bg_priority(2);
}

bn::fixed Pipe::x() const {
    return sprs[0].x();
}

void Pipe::set_x(bn::fixed x){
    sprs[0].set_x(x);
    sprs[1].set_x(x);
}

void Pipe::set_pos(bn::fixed_point position){
    sprs[0].set_position(position);
    sprs[1].set_position(position.x(), position.y() + 64);
    if(flipped) sprs[1].set_y(sprs[1].y() - 128);
}

bn::fixed_rect Pipe::hitbox() const {
    return bn::fixed_rect(sprs[1].x(), sprs[1].y() + ((flipped)? -32 : 32), 32, 256);
}


Pipes::Pipes() : // -32 and 32 makes them close at them middle
    pipes{
        Pipe(true, bn::fixed_point(136,-52)), 
        Pipe(false, bn::fixed_point(136,52)), 
        Pipe(true, bn::fixed_point(216,-52)), 
        Pipe(false, bn::fixed_point(216,52)),
        Pipe(true, bn::fixed_point(296,-36)), 
        Pipe(false, bn::fixed_point(296,68)), 
        Pipe(true, bn::fixed_point(376,-52)), 
        Pipe(false, bn::fixed_point(376,52))}
    {
}

void Pipes::update(){
    for(Pipe& pipe : pipes){
        pipe.set_x(pipe.x() - 1);
        if(pipe.x() < -183){
            pipe.set_x(136);
        }
    }
}

bool Pipes::check_collision(const bn::fixed_rect& bird_hitbox){
    for(Pipe& pipe : pipes){
        if(pipe.hitbox().intersects(bird_hitbox)){
            return true;
        }
    }
    return false;
}

void Pipes::reset_pipe(int index){
    pipes[index].set_x(140);
}