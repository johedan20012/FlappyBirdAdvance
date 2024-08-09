#include "pipes.h"

PipePair::PipePair(bn::fixed_point position, bn::fixed _gap) :
    half_gap(_gap/2),
    top_sprs{
        bn::sprite_items::pipe.create_sprite(position.x(), position.y()-half_gap-32), 
        bn::sprite_items::pipe.create_sprite(position.x(), position.y()-half_gap-96,1)},
    bottom_sprs{
        bn::sprite_items::pipe.create_sprite(position.x(), position.y()+half_gap+32), 
        bn::sprite_items::pipe.create_sprite(position.x(), position.y()+half_gap+96,1)}
    {
    top_sprs[0].set_vertical_flip(true);

    top_sprs[0].set_bg_priority(2);
    top_sprs[1].set_bg_priority(2);
    bottom_sprs[0].set_bg_priority(2);
    bottom_sprs[1].set_bg_priority(2);
}

bn::fixed PipePair::x() const {
    return top_sprs[0].x();
}

void PipePair::set_x(bn::fixed x){
    top_sprs[0].set_x(x);
    top_sprs[1].set_x(x);
    bottom_sprs[0].set_x(x);
    bottom_sprs[1].set_x(x);
}

void PipePair::set_y(bn::fixed y){
    top_sprs[0].set_y(y-half_gap-32);
    top_sprs[1].set_y(y-half_gap-96);
    bottom_sprs[0].set_y(y+half_gap+32);
    bottom_sprs[1].set_y(y+half_gap+96);
}

bool PipePair::check_collision(const bn::fixed_rect& bird_hitbox){
    bn::fixed_rect top_hitbox(top_sprs[0].x(), top_sprs[1].y()-32, 32, 256);
    bn::fixed_rect bottom_hitbox(bottom_sprs[0].x(), bottom_sprs[1].y()+32, 32, 256 );
    return top_hitbox.intersects(bird_hitbox) || bottom_hitbox.intersects(bird_hitbox);
}

bool PipePair::passed(const bn::fixed_rect& bird_hitbox){
    return bird_hitbox.x() >= top_sprs[0].x();
}


Pipes::Pipes(GlobalStuff& _global) : // -32 and 32 makes them close at them middle
    global(_global),
    pipes{
        PipePair(bn::fixed_point(136,-52)), 
        PipePair(bn::fixed_point(216,52)),
        PipePair(bn::fixed_point(296,68)), 
        PipePair(bn::fixed_point(376,52))},
    current_pipe(0)
    {
    for(int i = 0; i < pipes.size(); i++) set_random_y(i);
}

void Pipes::update(){
    for(int i = 0; i < pipes.size(); i++){
        pipes[i].set_x(pipes[i].x() - 1);
        if(pipes[i].x() < -183){
            pipes[i].set_x(136);
            set_random_y(i);
        }
    }
}

bool Pipes::check_collision(const bn::fixed_rect& bird_hitbox){
    for(PipePair& pipe : pipes){
        if(pipe.check_collision(bird_hitbox)) return true;
    }
    return false;
}

bool Pipes::check_passed(const bn::fixed_rect& bird_hitbox){
    if(pipes[current_pipe].passed(bird_hitbox)){
        current_pipe = (current_pipe + 1) % pipes.size();
        return true;
    }
    return false;
}

void Pipes::set_random_y(int index){
    // Middle point is in the range [-48,32] -> 80 - 48
    bn::fixed middle = global.rng().get_unbiased_fixed(81)-48;
    pipes[index].set_y(middle);
}