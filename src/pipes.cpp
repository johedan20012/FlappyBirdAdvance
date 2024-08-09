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

void Pipe::set_y(bn::fixed y){
    sprs[0].set_y(y);
    sprs[1].set_y(y + 64);
    if(flipped) sprs[1].set_y(sprs[1].y() - 128);
}

bn::fixed_rect Pipe::hitbox() const {
    return bn::fixed_rect(sprs[1].x(), sprs[1].y() + ((flipped)? -32 : 32), 32, 256);
}


Pipes::Pipes(GlobalStuff& _global) : // -32 and 32 makes them close at them middle
    global(_global),
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
    for(int i = 0; i < pipes.size(); i+=2) set_random_y(i);
    
}

void Pipes::update(){
    for(int i = 0; i < pipes.size(); i++){
        pipes[i].set_x(pipes[i].x() - 1);
        if(pipes[i].x() < -183){
            pipes[i].set_x(136);
            pipes[i+1].set_x(136);
            set_random_y(i);
            i++;
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

void Pipes::set_random_y(int index){
    // Middle point is in the range [-48,32] -> 80 - 48
    bn::fixed middle = global.rng().get_unbiased_fixed(81)-48;
    pipes[index].set_y(middle-52);
    pipes[index+1].set_y(middle+52);
}